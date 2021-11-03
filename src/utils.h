#include <exiv2/exiv2.hpp>

Exiv2::PreviewProperties getBiggestPreview(Exiv2::PreviewPropertiesList list)
{
    uint32_t curMaxSize = 0;
    uint32_t curMaxKey = 0;

    for (std::vector<Exiv2::PreviewProperties>::size_type i = 0; i != list.size(); i++)
    {
        if (list[i].size_ > curMaxSize)
        {
            curMaxSize = list[i].size_;
            curMaxKey = i;
        }
    }
    return list[curMaxKey];
}

std::vector<std::string> splitString(std::string str, char delimiter)
{
    std::vector<std::string> vect;
    std::stringstream stringStream(str);

    while (stringStream.good())
    {
        std::string substr;
        getline(stringStream, substr, '.');
        vect.push_back(substr);
    }

    return vect;
}