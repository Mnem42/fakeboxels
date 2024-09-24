#pragma once
#include <stdint.h>
#include <exception>

#define MAX_GRID_MEMUSAGE 16777216 //The actual grid memory usage is lower, but
                                   //extra space is needed for certain algorithms.
                                   //16777216 is 2^12 squared. This value is in
                                   //bytes.

namespace fakeboxels {
	namespace automata {
		template<typename T> class Grid {
		private:
			T* _value = NULL; //note: y is the wider axis
			int _xsz  = NULL; //note: unadjusted for margin
			int _ysz  = NULL; //note: unadjusted for margin
		public:
			Grid(uint16_t x, uint16_t y) {
				//Check if the grid is too large or too small
				if (((x - 2) * (y - 2) * sizeof(T)) > MAX_GRID_MEMUSAGE 
					|| x==0 || y==0)                                    
				{
					throw 1;
				}

				//If it is within limits, try doing the dynamic allocation
				
				//The format is kinda cursed, but it's abstracted anyway through
				//other functions

				//Check if it actually allocated
				if (_value == NULL) {
					throw 2;
				}

				//Actually set _xsz and _ysz
				_xsz = x;
				_ysz = y;

				//Yay, it didn't fail, and you have your very own grid!
				//(if it reaches this line, that is)
			}

			get_item_by_coord(uint16_t x, uint16_t y) {
				//Check if the coordinate is within bounds
				if (x>_xsz || y>_ysz)
				{
					throw 1;
				}

				//If it is within bounds, return the value
				return _value[y*_xsz+x]
			}
			set_item_by_coord(uint16_t x, uint16_t y, T value) {
				//Check if the coordinate is within bounds
				if (x > _xsz || y > _ysz)
				{
					throw 1;
				}

				//If it is within bounds, set the value
				_value[y * _xsz + x] = value;
			}
		};
	}
}