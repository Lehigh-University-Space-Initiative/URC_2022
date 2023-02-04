
import imgui

class GUIPanel:
    def __init__(self, panelName):
        self.panelName = panelName;

    def defaultWindowSize(self):
        return [800,500]

    # overiden by subclasses to draw pannel
    def drawPanel(self):
        pass

    # called by outside methods to render pannel into view
    def drawInView(self):
        # imgui.setNextWindowPos(imgui.ImVec2(self.defaultWindowsize()[0],self.defaultWindowSize()[1]),imgui.ImGuiCond_Once)
        imgui.begin(self.panelName, True)
        self.drawPanel(); 
        imgui.end()