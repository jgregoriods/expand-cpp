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
        void set_prob(int year);
        double get_prob();
        std::string get_name();
        int year; // remove
    private:
        std::string name;
        double x;
        double y;
        double prob;
        std::map<int, double> probs;
};

#endif