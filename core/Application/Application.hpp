#pragma once

#include "raylib-cpp.hpp"

#include "Canvas/Canvas.hpp"
#include "NeuralNetwork/NeuralNetwork.hpp"

#include <string>
#include <vector>
#include <random>
#include <deque>
#include "rlImGui.h"
#include "imgui.h"

class Application
{
public:
    Application(int width, int height, std::string_view windowName);
    ~Application();
    void Run();

private:
    void Update();
    void Render();
    void RunNeuralNetworkDemo();
    void TrainWithSampleData();
    std::vector<double> GenerateRandomImage();
    std::vector<double> CreateTargetVector(int digit);
    void RenderImGui();
    void RenderMetricsWindow();
    void UpdateMetrics();
    void CollectLayerStatistics();
    
    // Training parameters
    float m_learningRate = 0.01f;
    int m_currentActivation = 1; // 0=Sigmoid, 1=ReLU, 2=LeakyReLU, 3=ELU, 4=Tanh
    int m_currentInitialization = 1; // 0=Random, 1=Xavier, 2=He
    bool m_enableTraining = true;
    bool m_enableBatchNorm = false;
    float m_lossValue = 0.0f;
    
    // Metrics tracking
    struct MetricsData {
        std::deque<float> trainLoss;
        std::deque<float> accuracy;
        std::deque<float> learningRateHistory;
        std::vector<std::deque<float>> gradientNorms; // per layer
        std::vector<std::vector<float>> activationHist; // per layer
        std::vector<std::vector<float>> weightHist; // per layer
        std::vector<std::vector<float>> gradientHist; // per layer
        int maxHistorySize = 1000;
        int iteration = 0;
    } m_metrics;
    
    bool m_showMetricsWindow = true;
    int m_selectedLayer = 0;

private:
    int m_width;
    int m_height;

    // Track panning (offset) and zoom (scale)
    raylib::Vector2 m_offset = raylib::Vector2(0.0f, 0.0f);
    float m_scale = 1.0f;

    // For click-and-drag
    bool m_dragging = false;
    raylib::Vector2 m_lastMousePosition = {0.0f, 0.0f};

    std::string_view m_windowName;

    NeuralNetwork m_nn;

    raylib::Window m_window;
    
    // Helper functions for metrics
    float CalculateAccuracy(const std::vector<double>& outputs, const std::vector<double>& targets);
    void AddToHistory(std::deque<float>& history, float value);
    std::vector<float> CalculateHistogram(const std::vector<double>& data, int bins = 50);
};