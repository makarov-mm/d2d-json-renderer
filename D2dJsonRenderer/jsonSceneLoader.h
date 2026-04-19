#pragma once
#include <string>

#include "argbColor.h"
#include "point2D.h"
#include "scene.h"

namespace Infrastructure
{
    class JsonSceneLoader
    {
    public:
        std::string DisplayName() const { return "JSON..."; }
        std::string FileDialogFilter() const { return "JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0"; }

        bool CanLoad(const std::wstring& filePath) const;
        std::unique_ptr<Domain::Scene> Load(const std::wstring& filePath) const;

    private:
        Domain::ArgbColor   ParseColor(const std::string& value) const;
        Domain::Point2D     ParsePoint(const std::string& value) const;
        double              ParseDouble(const std::string& value) const;
    };
}
