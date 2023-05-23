#include <vector>
class Map
{

public:
    static const int MAX_MAP_WIDTH = 50;
    static const int MAX_MAP_HEGHIT = 21;
    int width;
    int height;
    int start_snake_length;
    int **data;
    Map(int height, int width, int newMap[MAX_MAP_HEGHIT][MAX_MAP_WIDTH], int start_snake_length) : width(width), height(height), start_snake_length(start_snake_length)
    {
        data = new int *[height];
        for (int i = 0; i < height; i++)
        {
            data[i] = new int[width];
            for (int j = 0; j < width; j++)
            {
                data[i][j] = newMap[i][j];
            }
        }
    }
    ~Map()
    {
        // for (int i = 0; i < height; i++)
        // {
        //     delete[] data[i];
        // }
        // delete[] data;
    }
};

class MapManager
{
public:
    MapManager();
    std::vector<Map> maps;
};