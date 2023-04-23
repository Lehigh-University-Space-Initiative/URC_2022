#pragma once
#include "../Panel.h"
#include "../GUI.h"
#include <vector>
#include <string>

class SoftwareDebugPanel: public Panel {
protected:
    virtual void drawBody() override;
public:
    using Panel::Panel;

    virtual void setup() override;
    virtual void update() override;

private:
    std::vector<std::string> debugInfoLines;
    // struct with 3 properties: ip address, host name, path to debug file
    // array of the struct called host define each computer
    // IN INIT METHOD populate all that
    std::chrono::system_clock::time_point lastUpdateTime = std::chrono::system_clock::now();
    const std::chrono::milliseconds refreshInterval = std::chrono::milliseconds(500);

    std::chrono::system_clock::time_point lastFileReadTime = std::chrono::system_clock::now();

    void readDebugInfoFile(const std::string& filePath);
};