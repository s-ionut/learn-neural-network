# gather all source files needed

set(CORE_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core)

include(${CORE_BASE_DIR}/Primitives/primitives.cmake)

set(SOURCE_FILES
    ${PRIMITIVES_SRC}
    ${CORE_BASE_DIR}/Helper/Helper.cpp
    ${CORE_BASE_DIR}/Application/Application.cpp
    ${CORE_BASE_DIR}/Neuron/Neuron.cpp
    ${CORE_BASE_DIR}/Layer/Layer.cpp
    ${CORE_BASE_DIR}/Connection/Connection.cpp
    ${CORE_BASE_DIR}/NeuralNetwork/NeuralNetwork.cpp
)