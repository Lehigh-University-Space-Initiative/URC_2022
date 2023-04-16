#pragma once
#include "../Panel.h"
#include "../GUI.h"

class SoftwareDebugPanel: public Panel {
protected:

    virtual void drawBody() override;

    

public:
    using Panel::Panel;

    virtual void setup() override;
    virtual void update() override;
};