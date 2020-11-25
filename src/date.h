#ifndef DATE_H
#define DATE_H

#include <map>
#include <string>

class Date {
    public:
        Date(std::string filename);
        void read_file(std::string filename);
        double get_x();
        double get_y();
        double get_prob(int year);
    private:
        double x;
        double y;
        std::map<int, double> probs;
};

#endif