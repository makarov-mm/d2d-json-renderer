#include "jsonSceneLoader.h"
#include "json.hpp"

#include <D2DBaseTypes.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <locale>

#include "circlePrimitive.h"
#include "linePrimitive.h"
#include "trianglePrimitive.h"

using json = nlohmann::json;

namespace Infrastructure
{
    static std::string WideToUtf8(const std::wstring& wide)
    {
        if (wide.empty()) return {};
        int size = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string result(size - 1, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, &result[0], size, nullptr, nullptr);
        return result;
    }

    static std::string Trim(const std::string& s)
    {
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end = s.find_last_not_of(" \t\r\n");
        if (start == std::string::npos) return {};
        return s.substr(start, end - start + 1);
    }

    static std::vector<std::string> Split(const std::string& s, char delim)
    {
        std::vector<std::string> parts;
        std::istringstream stream(s);
        std::string part;
        while (std::getline(stream, part, delim))
            parts.push_back(Trim(part));
        return parts;
    }

    static std::string ToLower(std::string s)
    {
        for (char& c : s)
            c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        return s;
    }

    bool JsonSceneLoader::CanLoad(const std::wstring& filePath) const
    {
        if (filePath.size() < 5) return false;
        std::wstring ext = filePath.substr(filePath.size() - 5);
        for (wchar_t& c : ext)
            c = towlower(c);
        return ext == L".json";
    }

    std::unique_ptr<Domain::Scene> JsonSceneLoader::Load(const std::wstring& filePath) const
    {
        std::ifstream file(filePath);
        if (!file.is_open())
            throw std::runtime_error("Cannot open file.");

        json items = json::parse(file);

        if (!items.is_array())
            throw std::runtime_error("Root JSON element must be an array.");

        auto scene = std::make_unique<Domain::Scene>();

        for (const auto& item : items)
        {
            std::string type = ToLower(item.value("type", ""));
            Domain::ArgbColor color = ParseColor(item.value("color", "255;0;0;0"));

            if (type == "line")
            {
                scene->Primitives.push_back(std::make_unique<Domain::LinePrimitive>(
                    ParsePoint(item.value("a", "0;0")),
                    ParsePoint(item.value("b", "0;0")),
                    color));
            }
            else if (type == "circle")
            {
                scene->Primitives.push_back(std::make_unique<Domain::CirclePrimitive>(
                    ParsePoint(item.value("center", "0;0")),
                    item.value("radius", 0.0),
                    item.value("filled", false),
                    color));
            }
            else if (type == "triangle")
            {
                scene->Primitives.push_back(std::make_unique<Domain::TrianglePrimitive>(
                    ParsePoint(item.value("a", "0;0")),
                    ParsePoint(item.value("b", "0;0")),
                    ParsePoint(item.value("c", "0;0")),
                    item.value("filled", false),
                    color));
            }
            else
            {
                throw std::runtime_error("Unsupported primitive type: " + type);
            }
        }

        return scene;
    }

    Domain::ArgbColor JsonSceneLoader::ParseColor(const std::string& value) const
    {
        auto parts = Split(value, ';');
        if (parts.size() != 4)
            throw std::runtime_error("Color must have 4 components: A;R;G;B");

        return Domain::ArgbColor(
            static_cast<unsigned char>(std::stoi(parts[0])),
            static_cast<unsigned char>(std::stoi(parts[1])),
            static_cast<unsigned char>(std::stoi(parts[2])),
            static_cast<unsigned char>(std::stoi(parts[3])));
    }

    Domain::Point2D JsonSceneLoader::ParsePoint(const std::string& value) const
    {
        auto parts = Split(value, ';');
        if (parts.size() != 2)
            throw std::runtime_error("Point must have 2 components: X;Y");

        return Domain::Point2D(ParseDouble(parts[0]), ParseDouble(parts[1]));
    }

    double JsonSceneLoader::ParseDouble(const std::string& value) const
    {
        std::string normalized = value;

        for (char& c : normalized)
            if (c == ',') c = '.';

        return std::stod(normalized);
    }
}
