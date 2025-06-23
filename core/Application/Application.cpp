#include "Application.hpp"
#include "imgui.h"

Application::Application(int width, int height, std::string_view windowName)
    : m_width{width},
      m_height{height},
      m_windowName{windowName},
      m_window(m_width, m_height, std::string{m_windowName}),
      m_nn(4, std::vector<int>{16, 3, 2, 10})
{
    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60);

    int canvasWidth = 1400;
    int canvasHeight = 1000;

    Canvas::getInstance().Resize(canvasWidth, canvasHeight);
    m_nn.Resize(0, 0, canvasWidth, canvasHeight);

    // Initialize ImGui with proper setup
    rlImGuiSetup(true);

    // Force font atlas build
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->Build();

    // Set default activation to ReLU
    m_nn.SetActivationFunction(Neuron::ActivationType::RELU);

    // Initialize metrics
    m_metrics.gradientNorms.resize(m_nn.GetNumLayers() - 1);
    m_metrics.activationHist.resize(m_nn.GetNumLayers());
    m_metrics.weightHist.resize(m_nn.GetNumLayers() - 1);
    m_metrics.gradientHist.resize(m_nn.GetNumLayers());
};

Application::~Application()
{
    rlImGuiShutdown();
}

void Application::Run()
{
    while (!m_window.ShouldClose())
    {
        Update();
        Render();
    }
};

void Application::Update()
{
    // -- Handle Zoom --
    float mouseWheel = GetMouseWheelMove();
    if (mouseWheel != 0.0f)
    {
        m_scale += mouseWheel * 0.1f;
        if (m_scale < 0.1f)
            m_scale = 0.1f;
    }

    // -- Handle Click & Drag to Pan --
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        m_dragging = true;
        m_lastMousePosition = GetMousePosition();
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        m_dragging = false;
    }

    if (m_dragging)
    {
        raylib::Vector2 currentMousePosition = GetMousePosition();

        raylib::Vector2 delta = currentMousePosition - m_lastMousePosition;

        m_offset += delta;

        m_lastMousePosition = currentMousePosition;
    }

    // Run neural network with sample data
    if (m_enableTraining)
    {
        RunNeuralNetworkDemo();
    }
    else
    {
        // Even when not training, update metrics for current state
        UpdateMetrics();
    }

    m_nn.Update();
    // Canvas no longer needed for direct rendering
};

void Application::RunNeuralNetworkDemo()
{
    // Train neural network with sample data every few frames
    static int frameCounter = 0;
    frameCounter++;

    if (frameCounter % 2 == 0) // Every 2 frames
    {
        TrainWithSampleData();
    }
};

void Application::TrainWithSampleData()
{
    // Generate random "handwritten digit" data for demonstration
    std::vector<double> inputs = GenerateRandomImage();

    // Random target digit (0-9)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 9);
    int targetDigit = dis(gen);

    std::vector<double> targets = CreateTargetVector(targetDigit);

    // Train the network with current learning rate
    m_nn.Train(inputs, targets, m_learningRate);

    // Also run forward pass to see current output and calculate loss
    std::vector<double> outputs = m_nn.Forward(inputs);
    m_lossValue = static_cast<float>(m_nn.CalculateLoss(outputs, targets));

    // Update metrics
    UpdateMetrics();
};

std::vector<double> Application::GenerateRandomImage()
{
    // Generate random input data for 16-input network
    std::vector<double> inputs(16);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < 16; i++)
    {
        inputs[i] = dis(gen);
    }

    return inputs;
};

std::vector<double> Application::CreateTargetVector(int digit)
{
    // Create one-hot encoded target vector for digits 0-9
    std::vector<double> target(10, 0.0);
    if (digit >= 0 && digit <= 9)
    {
        target[digit] = 1.0;
    }
    return target;
};

void Application::Render()
{
    BeginDrawing();

    m_window.ClearBackground(raylib::Color::Black());

    // Apply camera transform for zoom/pan
    BeginMode2D(raylib::Camera2D{m_offset, {0, 0}, 0.0f, m_scale});

    // Draw neural network with vector graphics (not canvas)
    m_nn.DrawDirect();

    EndMode2D();

    rlImGuiBegin();

    // Render ImGui controls
    RenderImGui();

    // Render metrics window
    if (m_showMetricsWindow)
    {
        RenderMetricsWindow();
    }
    rlImGuiEnd();

    EndDrawing();
};

void Application::RenderImGui()
{

    // Check if ImGui is ready
    ImGuiIO &io = ImGui::GetIO();
    if (!io.Fonts->IsBuilt())
    {
        printf("Warning: ImGui fonts not built, rebuilding...\n");
        io.Fonts->Build();
    }

    // Main control window
    if (ImGui::Begin("Neural Network Controls"))
    {
        ImGui::Text("Training Parameters");
        ImGui::Separator();

        // Learning rate slider
        if (ImGui::SliderFloat("Learning Rate", &m_learningRate, 0.001f, 1.0f, "%.4f"))
        {
            // Learning rate changed
        }

        // Activation function selection
        const char *activations[] = {"Sigmoid", "ReLU", "Leaky ReLU", "ELU", "Tanh"};
        if (ImGui::Combo("Activation Function", &m_currentActivation, activations, 5))
        {
            Neuron::ActivationType newActivation;
            switch (m_currentActivation)
            {
            case 0:
                newActivation = Neuron::ActivationType::SIGMOID;
                break;
            case 1:
                newActivation = Neuron::ActivationType::RELU;
                break;
            case 2:
                newActivation = Neuron::ActivationType::LEAKY_RELU;
                break;
            case 3:
                newActivation = Neuron::ActivationType::ELU;
                break;
            case 4:
                newActivation = Neuron::ActivationType::TANH;
                break;
            default:
                newActivation = Neuron::ActivationType::RELU;
                break;
            }
            m_nn.SetActivationFunction(newActivation);
        }

        // Weight initialization
        const char *initializations[] = {"Random", "Xavier", "He"};
        if (ImGui::Combo("Weight Initialization", &m_currentInitialization, initializations, 3))
        {
            NeuralNetwork::InitializationType newInit;
            switch (m_currentInitialization)
            {
            case 0:
                newInit = NeuralNetwork::InitializationType::RANDOM;
                break;
            case 1:
                newInit = NeuralNetwork::InitializationType::XAVIER;
                break;
            case 2:
                newInit = NeuralNetwork::InitializationType::HE;
                break;
            default:
                newInit = NeuralNetwork::InitializationType::HE;
                break;
            }
            m_nn.InitializeWeights(newInit);
        }

        ImGui::Separator();

        // Training control
        ImGui::Checkbox("Enable Training", &m_enableTraining);

        // Batch normalization
        if (ImGui::Checkbox("Batch Normalization", &m_enableBatchNorm))
        {
            // Enable/disable batch norm for all neurons
            // This would be implemented in the neural network class
        }

        // Display current loss
        ImGui::Text("Current Loss: %.6f", m_lossValue);

        ImGui::Separator();
        ImGui::Text("Controls:");
        ImGui::BulletText("Mouse Drag: Pan view");
        ImGui::BulletText("Mouse Wheel: Zoom");
        ImGui::BulletText("Colors: Green=Positive weights, Red=Negative");
        ImGui::BulletText("Neuron colors show activation levels");
    }
    ImGui::End();
};

void Application::UpdateMetrics()
{
    m_metrics.iteration++;

    // Add loss to history
    AddToHistory(m_metrics.trainLoss, m_lossValue);

    // Add learning rate to history
    AddToHistory(m_metrics.learningRateHistory, m_learningRate);

    // Calculate and add accuracy (simple random for demo)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    float accuracy = static_cast<float>(dis(gen));
    AddToHistory(m_metrics.accuracy, accuracy);

    // Collect layer statistics
    CollectLayerStatistics();
};

void Application::CollectLayerStatistics()
{
    // Get gradient norms for each layer
    std::vector<double> gradNorms = m_nn.GetGradientNorms();
    for (size_t i = 0; i < gradNorms.size() && i < m_metrics.gradientNorms.size(); i++)
    {
        AddToHistory(m_metrics.gradientNorms[i], static_cast<float>(gradNorms[i]));
    }

    // Update histograms for each layer
    for (int layer = 0; layer < m_nn.GetNumLayers(); layer++)
    {
        // Activation histogram
        std::vector<double> activations = m_nn.GetLayerActivations(layer);
        m_metrics.activationHist[layer] = CalculateHistogram(activations);

        // Weight histogram (for all layers except last)
        if (layer < m_nn.GetNumLayers() - 1)
        {
            std::vector<double> weights = m_nn.GetLayerWeights(layer);
            m_metrics.weightHist[layer] = CalculateHistogram(weights);
        }

        // Gradient histogram
        std::vector<double> gradients = m_nn.GetLayerGradients(layer);
        m_metrics.gradientHist[layer] = CalculateHistogram(gradients);
    }
};

float Application::CalculateAccuracy(const std::vector<double> &outputs, const std::vector<double> &targets)
{
    if (outputs.empty() || targets.empty())
        return 0.0f;

    // Find max indices
    int maxOutputIdx = 0, maxTargetIdx = 0;
    for (size_t i = 1; i < outputs.size(); i++)
    {
        if (outputs[i] > outputs[maxOutputIdx])
            maxOutputIdx = i;
    }
    for (size_t i = 1; i < targets.size(); i++)
    {
        if (targets[i] > targets[maxTargetIdx])
            maxTargetIdx = i;
    }

    return (maxOutputIdx == maxTargetIdx) ? 1.0f : 0.0f;
};

void Application::AddToHistory(std::deque<float> &history, float value)
{
    history.push_back(value);
    if (history.size() > m_metrics.maxHistorySize)
    {
        history.pop_front();
    }
};

std::vector<float> Application::CalculateHistogram(const std::vector<double> &data, int bins)
{
    std::vector<float> histogram(bins, 0.0f);
    if (data.empty())
        return histogram;

    // Find min and max
    double minVal = *std::min_element(data.begin(), data.end());
    double maxVal = *std::max_element(data.begin(), data.end());

    if (maxVal == minVal)
    {
        if (bins > 0)
            histogram[bins / 2] = static_cast<float>(data.size());
        return histogram;
    }

    // Fill histogram
    double range = maxVal - minVal;
    for (double value : data)
    {
        int bin = static_cast<int>((value - minVal) / range * (bins - 1));
        bin = std::max(0, std::min(bins - 1, bin));
        histogram[bin] += 1.0f;
    }

    return histogram;
};

void Application::RenderMetricsWindow()
{
    ImGui::SetNextWindowSize(ImVec2(900, 700), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Neural Network Metrics Dashboard", &m_showMetricsWindow))
    {
        // Header with key stats
        ImGui::Text("Training Metrics - Iteration: %d", m_metrics.iteration);
        ImGui::SameLine();
        ImGui::Text(" | Loss: %.6f | LR: %.4f", m_lossValue, m_learningRate);
        ImGui::Separator();

        // Create tabs for organized metrics
        if (ImGui::BeginTabBar("MetricsTabs", ImGuiTabBarFlags_None))
        {
            // === LOSS & ACCURACY TAB ===
            if (ImGui::BeginTabItem("Loss & Accuracy"))
            {
                // === Training Loss ===
                ImGui::Text("Training Loss (%zu samples)", m_metrics.trainLoss.size());
                if (!m_metrics.trainLoss.empty())
                {
                    std::vector<float> lossData(m_metrics.trainLoss.begin(), m_metrics.trainLoss.end());
                    ImGui::PlotLines("##TrainLoss", lossData.data(),
                                     static_cast<int>(lossData.size()),
                                     0, "Training Loss", FLT_MAX, FLT_MAX,
                                     ImVec2(800, 120));
                    ImGui::Text("Current: %.6f | Min: %.6f | Max: %.6f",
                                lossData.back(),
                                *std::min_element(lossData.begin(), lossData.end()),
                                *std::max_element(lossData.begin(), lossData.end()));
                }
                else
                {
                    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Collecting loss data...");
                }

                // add space or separator
                ImGui::Separator();
                ImGui::NewLine(); // optional

                // === Accuracy ===
                ImGui::Text("Accuracy (%zu samples)", m_metrics.accuracy.size());
                if (!m_metrics.accuracy.empty())
                {
                    std::vector<float> accData(m_metrics.accuracy.begin(), m_metrics.accuracy.end());
                    ImGui::PlotLines("##Accuracy", accData.data(),
                                     static_cast<int>(accData.size()),
                                     0, "Accuracy", 0.0f, 1.0f,
                                     ImVec2(800, 120));
                    ImGui::Text("Current: %.3f | Average: %.3f",
                                accData.back(),
                                std::accumulate(accData.begin(), accData.end(), 0.0f) / accData.size());
                }
                else
                {
                    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Collecting accuracy data...");
                }
                ImGui::Separator();

                // Learning Rate Schedule
                ImGui::Text("Learning Rate Schedule (%zu samples)", m_metrics.learningRateHistory.size());
                if (!m_metrics.learningRateHistory.empty())
                {
                    std::vector<float> lrData(m_metrics.learningRateHistory.begin(), m_metrics.learningRateHistory.end());
                    ImGui::PlotLines("##LearningRate", lrData.data(), static_cast<int>(lrData.size()),
                                     0, "Learning Rate", FLT_MAX, FLT_MAX, ImVec2(800, 100));
                }

                ImGui::EndTabItem();
            }

            // === GRADIENT NORMS TAB ===
            if (ImGui::BeginTabItem("Gradient Analysis"))
            {
                ImGui::Text("Gradient Norms by Layer (detecting vanishing/exploding gradients)");
                ImGui::Separator();

                for (size_t layer = 0; layer < m_metrics.gradientNorms.size(); layer++)
                {
                    if (!m_metrics.gradientNorms[layer].empty())
                    {
                        std::vector<float> gradData(m_metrics.gradientNorms[layer].begin(), m_metrics.gradientNorms[layer].end());

                        // Color code based on gradient magnitude
                        float currentGrad = gradData.back();
                        ImVec4 color = ImVec4(1, 1, 1, 1); // white
                        if (currentGrad < 1e-6f)
                            color = ImVec4(1, 0, 0, 1); // red for vanishing
                        else if (currentGrad > 1.0f)
                            color = ImVec4(1, 0.5f, 0, 1); // orange for exploding
                        else
                            color = ImVec4(0, 1, 0, 1); // green for healthy

                        ImGui::TextColored(color, "Layer %zu (samples: %zu)", layer + 1, gradData.size());
                        ImGui::SameLine();
                        ImGui::Text("Current: %.2e", currentGrad);

                        std::string plotLabel = "##GradLayer" + std::to_string(layer);
                        ImGui::PlotLines(plotLabel.c_str(), gradData.data(), static_cast<int>(gradData.size()),
                                         0, nullptr, FLT_MAX, FLT_MAX, ImVec2(800, 80));

                        // Warning indicators
                        if (currentGrad < 1e-6f)
                        {
                            ImGui::SameLine();
                            ImGui::TextColored(ImVec4(1, 0, 0, 1), "⚠️ VANISHING");
                        }
                        else if (currentGrad > 1.0f)
                        {
                            ImGui::SameLine();
                            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "⚠️ EXPLODING");
                        }
                    }
                }

                ImGui::EndTabItem();
            }

            // === HISTOGRAMS TAB ===
            if (ImGui::BeginTabItem("Distributions"))
            {
                // Layer selector
                ImGui::SliderInt("Layer", &m_selectedLayer, 0, m_nn.GetNumLayers() - 1);
                ImGui::SameLine();
                ImGui::Text("(Layer %d: %d neurons)", m_selectedLayer, m_nn.GetLayerSize(m_selectedLayer));
                ImGui::Separator();

                if (m_selectedLayer >= 0 && m_selectedLayer < m_nn.GetNumLayers())
                {
                    ImGui::Columns(2, "HistColumns", false);

                    // Activation Histogram
                    ImGui::Text("Activation Distribution");
                    if (m_selectedLayer < m_metrics.activationHist.size() && !m_metrics.activationHist[m_selectedLayer].empty())
                    {
                        ImGui::PlotHistogram("##ActivationHist", m_metrics.activationHist[m_selectedLayer].data(),
                                             static_cast<int>(m_metrics.activationHist[m_selectedLayer].size()),
                                             0, "Activations", FLT_MAX, FLT_MAX, ImVec2(400, 150));

                        // Check for saturation
                        auto &hist = m_metrics.activationHist[m_selectedLayer];
                        float leftEdge = hist[0] + hist[1] + hist[2];
                        float rightEdge = hist[hist.size() - 3] + hist[hist.size() - 2] + hist[hist.size() - 1];
                        float total = std::accumulate(hist.begin(), hist.end(), 0.0f);

                        if (leftEdge / total > 0.3f)
                            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Dead neurons detected!");
                        if (rightEdge / total > 0.3f)
                            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Saturation detected!");
                    }
                    else
                    {
                        ImGui::Text("No activation data for layer %d", m_selectedLayer);
                    }

                    ImGui::NextColumn();

                    // Weight Histogram
                    if (m_selectedLayer < m_nn.GetNumLayers() - 1)
                    {
                        ImGui::Text("Weight Distribution");
                        if (m_selectedLayer < m_metrics.weightHist.size() && !m_metrics.weightHist[m_selectedLayer].empty())
                        {
                            ImGui::PlotHistogram("##WeightHist", m_metrics.weightHist[m_selectedLayer].data(),
                                                 static_cast<int>(m_metrics.weightHist[m_selectedLayer].size()),
                                                 0, "Weights", FLT_MAX, FLT_MAX, ImVec2(400, 150));
                        }
                    }

                    ImGui::Columns(1);
                    ImGui::Separator();

                    // Gradient Histogram for selected layer
                    ImGui::Text("Gradient Distribution - Layer %d", m_selectedLayer);
                    if (m_selectedLayer < m_metrics.gradientHist.size() && !m_metrics.gradientHist[m_selectedLayer].empty())
                    {
                        ImGui::PlotHistogram("##GradientHistSelected", m_metrics.gradientHist[m_selectedLayer].data(),
                                             static_cast<int>(m_metrics.gradientHist[m_selectedLayer].size()),
                                             0, "Gradients", FLT_MAX, FLT_MAX, ImVec2(800, 120));
                    }
                }

                ImGui::EndTabItem();
            }

            // === NETWORK HEALTH TAB ===
            if (ImGui::BeginTabItem("Health Monitor"))
            {
                ImGui::Text("Network Health Indicators");
                ImGui::Separator();

                // Overall health score
                float healthScore = 1.0f;
                ImVec4 healthColor = ImVec4(0, 1, 0, 1);

                // Check gradient health
                bool hasVanishing = false, hasExploding = false;
                for (size_t layer = 0; layer < m_metrics.gradientNorms.size(); layer++)
                {
                    if (!m_metrics.gradientNorms[layer].empty())
                    {
                        float grad = m_metrics.gradientNorms[layer].back();
                        if (grad < 1e-6f)
                            hasVanishing = true;
                        if (grad > 1.0f)
                            hasExploding = true;
                    }
                }

                ImGui::Text("Gradient Status:");
                if (hasVanishing)
                {
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "VANISHING GRADIENTS");
                    healthScore *= 0.3f;
                }
                if (hasExploding)
                {
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "EXPLODING GRADIENTS");
                    healthScore *= 0.5f;
                }
                if (!hasVanishing && !hasExploding)
                {
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0, 1, 0, 1), "HEALTHY");
                }

                // Learning progress
                ImGui::Text("Learning Progress:");
                if (!m_metrics.trainLoss.empty() && m_metrics.trainLoss.size() > 10)
                {
                    float recentLoss = std::accumulate(m_metrics.trainLoss.end() - 10, m_metrics.trainLoss.end(), 0.0f) / 10.0f;
                    float oldLoss = std::accumulate(m_metrics.trainLoss.begin(), m_metrics.trainLoss.begin() + 10, 0.0f) / 10.0f;
                    float improvement = (oldLoss - recentLoss) / oldLoss;

                    ImGui::SameLine();
                    if (improvement > 0.1f)
                        ImGui::TextColored(ImVec4(0, 1, 0, 1), "IMPROVING (%.1f%%)", improvement * 100);
                    else if (improvement < -0.1f)
                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "DEGRADING (%.1f%%)", improvement * 100);
                    else
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), "STABLE");
                }

                // Overall health bar
                if (healthScore < 0.3f)
                    healthColor = ImVec4(1, 0, 0, 1);
                else if (healthScore < 0.7f)
                    healthColor = ImVec4(1, 1, 0, 1);

                ImGui::Text("Overall Health:");
                ImGui::SameLine();
                ImGui::ProgressBar(healthScore, ImVec2(300, 0), "");
                ImGui::SameLine();
                ImGui::TextColored(healthColor, "%.0f%%", healthScore * 100);

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
};