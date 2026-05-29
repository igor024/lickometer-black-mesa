#pragma once

#include <constants.hpp>
#include <data/lick.hpp>

class cage_data {
    private:
        
        lick bout[100]; //stores the current unwritten flush_bout
        lick current_lick; //stores the current lick
        
    public:
        cage_data();

        /*
         * records animal starts licking
         * will reset the bout if lick doesn't meet conditions
         */

        void record_contact_in();

        /*
         * records animal starts licking
         * will reset the bout if lick doesn't meet conditions
         */

        void record_contact_out();
        
        /*
         * writes remaining bout to SD as is, even if the bout is incomplete
         * to be called once to flush the last bout before saving
         */

        void flush_bout();
};

