#ifndef RMDISK_H
# define RMDISK_H

#include "string"

using namespace std;

class rmdisk{
    public:
        rmdisk();
        string path;
        void driver(rmdisk *disco);
        void _delete(rmdisk *disco);
};

#endif