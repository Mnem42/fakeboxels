#pragma once
#include <stdint.h>
#include <stdexcept>

#define MAX_GRID_MEMUSAGE 128 //The actual grid memory usage is lower, but
                                   //extra space is needed for certain algorithms.
                                   //16777216 is 2^12 squared. This value is in
                                   //bytes.

namespace fakeboxels {
	namespace automata {
		template<typename T> class Grid {
		private:
			T* _value = NULL; //note: y is the wider axis
			uint16_t _xsz  = NULL; //note: unadjusted for margin
			uint16_t _ysz  = NULL; //note: unadjusted for margin
		public:
			Grid(uint16_t x, uint16_t y) { //error flags 

				//Check if the grid is too large or too small
				// 
				//It throws an exception if it's out of bounds, because
				// - It should be called rarely anyway
				// - It shouldn't really happen
				if (((x + 2) * (y + 2) * sizeof(T)) > MAX_GRID_MEMUSAGE 
					|| x==0 || y==0)                                    
				{
					throw std::invalid_argument("Input not in range");
				}

				//The array format is kinda cursed, but it's abstracted anyway, through
				//other functions
				// 
				//Check if it actually allocated. Will throw an exception because of new
				_value = new T[(x + 2) * (y + 2)];

				//zero initialise everything
				for (int i = 0; i < (x + 2)*(y + 2);i++) {
					_value[i] = 0;
				}

				//Actually set _xsz and _ysz
				_xsz = x;
				_ysz = y;

				//Yay, it didn't fail, and you have your very own grid!
				//(if it reaches this line, that is)
			}

			T get_item_by_coord(uint16_t x, uint16_t y) {
				//Check if the coordinate is within bounds
				if (x>_xsz || y>_ysz)
				{
					throw std::invalid_argument("Access outside possible bounds");
				}

				//If it is within bounds, return the value
				return _value[y * _xsz + x];
			}
			void set_item_by_coord(uint16_t x, uint16_t y, T value) {
				//Check if the coordinate is within bounds
				if (x > _xsz || y > _ysz)
				{
					throw std::invalid_argument("Access outside possible bounds");
				}

				//If it is within bounds, set the value
				_value[y * _xsz + x] = value;
			}
		};
	}
}