#pragma once
#include <stdint.h>
#include <stdexcept>

#define MAX_GRID_MEMUSAGE 128 //The actual grid memory usage is lower, but
                                   //extra space is needed for certain algorithms.
                                   //16777216 is 2^12 squared. This value is in
                                   //bytes.

namespace fakeboxels {
	namespace utils {
		struct Coord2D {
			uint16_t x;
			uint16_t y;
		};
	}
	namespace automata {

		//Automata handling stuff
		template<class T,typename ItemType> concept AutomataHandler = requires(T a)
		{
			{ a.run_automata(new Grid<ItemType>(1,1)) } -> std::convertible_to<uint16_t[9]>;
		};
		//Exists for... reasons
		template<class T> concept AutomataCell = requires(T a) {
			{ a.type_id } -> std::convertible_to<uint16_t>;
		};

		//Grid type for automata stuff
		template<typename T> class Grid {
		private:
			T* _value      = NULL;      //note: y is the wider axis
			uint16_t _xsz  = NULL; //note: unadjusted for margin
			uint16_t _ysz  = NULL; //note: unadjusted for margin
		public:
			//Constructors which also do some allocation. There's an overload which
			//uses coordinates instead
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
			Grid(fakeboxels::utils::Coord2D size) { //error flags 

				//Check if the grid is too large or too small
				// 
				//It throws an exception if it's out of bounds, because
				// - It should be called rarely anyway
				// - It shouldn't really happen
				if (((size.x + 2) * (size.y + 2) * sizeof(T)) > MAX_GRID_MEMUSAGE
					|| size.x == 0 || size.y == 0)
				{
					throw std::invalid_argument("Input not in range");
				}

				//The array format is kinda cursed, but it's abstracted anyway, through
				//other functions
				// 
				//Check if it actually allocated. Will throw an exception because of new
				_value = new T[(size.x + 2) * (size.y + 2)];

				//zero initialise everything
				for (int i = 0; i < (size.x + 2) * size.(y + 2); i++) {
					_value[i] = 0;
				}

				//Actually set _xsz and _ysz
				_xsz = size.x;
				_ysz = size.y;

				//Yay, it didn't fail, and you have your very own grid!
				//(if it reaches this line, that is)
			}

			//Do exactly what they say on the tin
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

			//Some overloads to use coords instead of seperate uints
			T get_item_by_coord(fakeboxels::utils::Coord2D coord) {
				//Check if the coordinate is within bounds
				if (coord.x > _xsz || coord.y > _ysz)
				{
					throw std::invalid_argument("Access outside possible bounds");
				}

				//If it is within bounds, return the value
				return _value[coord.y * _xsz + coord.x];
			}
			void set_item_by_coord(fakeboxels::utils::Coord2D coord, T value) {
				//Check if the coordinate is within bounds
				if (coord.x > _xsz || coord.y > _ysz)
				{
					throw std::invalid_argument("Access outside possible bounds");
				}

				//If it is within bounds, set the value
				_value[coord.y * _xsz + coord.x] = value;
			}

			//Hands the grid over to an actual handler for the automata (modularity FTW)
			template<AutomataHandler Handler> void run_automata_iter() {
				Handler.do_automata(this);
			}

			//obligatory access function //obligatory function to grab _xsz and _ysz
			fakeboxels::utils::Coord2D grab_size() {
				return { _xsz,_ysz };
			} 
		};
	}
}