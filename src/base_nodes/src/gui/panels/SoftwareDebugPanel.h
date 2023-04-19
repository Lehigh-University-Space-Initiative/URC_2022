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
    double lastUpdateTime;
    const double refreshInterval = 0.5; // seconds

    void readDebugInfoFile(const std::string& filePath);
};