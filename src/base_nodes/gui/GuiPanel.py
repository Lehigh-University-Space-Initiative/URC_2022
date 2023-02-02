
class GUIPanel:
    def __init__(self, panelName):
        self.panelName = panelName;


    # overiden by subclasses to draw pannel
    def drawPanel(self):
        pass

    # called by outside methods to render pannel into view
    def drawInView(self):
        self.drawPanel(); 