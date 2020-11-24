#ifndef DATE_H
#define DATE_H

#include <map>
#include <string>
#include <fstream>

class Date {
    public:
        double x, y;
        std::map<int, double> probs;
        Date(std::string filename);
        void read_file(std::string filename);
};

#endif