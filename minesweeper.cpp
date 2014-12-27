#include "minesweeper.h"

int main( int argc, char **argv )
{
  int CEAT_MODE = atoi(argv[1]);
  Minesweeper ms;
  while ( true )
	{
	  ms.display_field();
	  if ( CEAT_MODE ) ms.display_data();
	  ms.receive_command();
	  ms.execute_command();
	  if ( ms.judge_game() )
		{
		  ms.send_message();
		  break;
		}
	}
  return EXIT_SUCCESS;
}

Minesweeper::Minesweeper()
{
  std::cout << "Minesweeper Start!!" << std::endl;
  do
	{
	  std::cout << "Enter field size:" << std::endl;
	  std::cin >> field_size;
	} while ( field_size < 0 || field_size > 4294967295 );
  do
	{
	  std::cout << "Enter the number of mines:" << std::endl;
	  std::cin >> num_mine;
	} while ( num_mine > field_size * field_size );
  field_data.resize(field_size);
  field_display.resize(field_size);
  field_check.resize(field_size);
  for ( int i = 0 ; i < field_size ; ++i )
	{
	  field_data[i].assign(field_size, '.'); 
	  field_display[i].assign(field_size, '?');
	  field_check[i].assign(field_size, '_');
	}
  int cnt_mine = 0;
  std::random_device seed_generator;
  std::mt19937 engine(seed_generator());
  std::uniform_real_distribution<> dist(0, field_size);
  while ( cnt_mine < num_mine )
	{
	  int i = dist(engine);
	  int j = dist(engine);
	  if ( field_data[i][j] == '.' )
		{
		  field_data[i][j] = '*';
		  cnt_mine++;
		}
	}
  num_open_block = 0;
}

Minesweeper::~Minesweeper()
{
}

void Minesweeper::display_field()
{
  std::cout << "  ";
  for ( int i = 0 ; i < field_size ; ++i )
	{
	  std::cout << std::setw(2) << i;
	}
  std::cout << std::endl;
  for ( int i = 0 ; i < field_size ; ++i )
	{
	  std::cout << std::setw(2) << i;
	  for ( int j = 0 ; j < field_size ; ++ j )
		{
		  std::cout << std::setw(2) << field_display[i][j];
		}
	  std::cout << std::endl;
	}
}

void Minesweeper::receive_command()
{
  std::cout << "Select row and column:" << std::endl;
  std::cout << "usage: row colmun" << std::endl;
  std::cin >> input_row >> input_column;
  std::cout << "If you want to open, enter o." << std::endl;
  std::cout << "If you want to check, enter x." << std::endl;
  std::cin >> command;
}

void Minesweeper::execute_command()
{
  if ( command == 'o' )
	{
	  if ( field_display[input_row][input_column] == 'x' )
		{
		  status = invalid;
		}
	  else if (field_display[input_row][input_column] != 'x' && 
			   field_display[input_row][input_column] != '?' ) 
		{
		  status = invalid;
		}
	  else if ( field_data[input_row][input_column] == '*' ) 
		{
		  status = failure;
		}
	  else if ( field_display[input_row][input_column] == '?' )
		{
		  int di[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
		  int dj[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
		  int num_mines_neighborhood = 0;
		  for ( int k = 0 ; k < 8 ; ++k )
			{
			  if ( input_row + di[k] >= 0 &&  
				   input_row + di[k] < field_size && 
				   input_column + dj[k] >= 0 && 
				   input_column + dj[k] < field_size && 
				   field_data[input_row+di[k]][input_column+dj[k]] == '*' )
				{
				  num_mines_neighborhood++;
				}
			}
		  field_display[input_row][input_column] = (num_mines_neighborhood + '0');
		  if (num_mines_neighborhood == 0 )
			{
			  for ( int k = 0 ; k < field_size ; ++k )
				{
				  field_check[k].assign(field_size, '_');
				}
			  field_check[input_row][input_column] = '@';
			  std::cout << "start" << std::endl;
			  open_neighborhood(input_row, input_column);

			}
		  status = safe;
		}
	}
  else if ( command == 'x' )
	{
	  if ( field_display[input_row][input_column] == '?' )
		{
		  field_display[input_row][input_column] = 'x';
		  status = safe;
		}
	  else if ( field_display[input_row][input_column] == 'x' )
		{
		  field_display[input_row][input_column] = '?';
		  status = safe;
		}
	  else if (field_display[input_row][input_column] != 'x' && 
			   field_display[input_row][input_column] != '?' ) 
		{
		  status = invalid;
		}
	}
}

bool Minesweeper::judge_game()
{
  num_open_block = 0;
  for ( int i = 0 ; i < field_size ; ++i )
	{
	  for ( int j = 0 ; j < field_size ; ++j )
		{
		  if ( field_display[i][j] != '?' &&
			   field_display[i][j] != 'x' )
			{
			  num_open_block++;
			}
		}
	}

  if ( field_size * field_size - num_mine == num_open_block )
	{
	  status = success;
	  return true;
	}
  if (status == failure )
	{
	  return true;
	}
  return false;
}

void Minesweeper::send_message()
{
  std::cout << "The number of open block is " << num_open_block << std::endl;
  if ( status == success )
	{
	  std::cout << "Congratulations!" << std::endl;
	}
  else if ( status == failure )
	{
	  std::cout << "Bad luck!" << std::endl;
	}
}

void Minesweeper::display_data()
{
  std::cout << "  ";
  for ( int i = 0 ; i < field_size ; ++i )
	{
	  std::cout << std::setw(2) << i;
	}
  std::cout << std::endl;
  for ( int i = 0 ; i < field_size ; ++i )
	{
	  std::cout << std::setw(2) << i;
	  for ( int j = 0 ; j < field_size ; ++ j )
		{
		  std::cout << std::setw(2) << field_data[i][j];
		}
	  std::cout << std::endl;
	}
}

void Minesweeper::open_neighborhood( int cent_row, int cent_column )
{
  int di[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dj[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
  for ( int k = 0 ; k < 8 ; ++k )
	{
	  if ( cent_row + di[k] >= 0 &&  
		   cent_row + di[k] < field_size && 
		   cent_column + dj[k] >= 0 && 
		   cent_column + dj[k] < field_size )
		{
		  int cent_cent_row = cent_row + di[k];
		  int cent_cent_column = cent_column + dj[k];
		  int num_mines_neighborhood = 0;
  		  for ( int m = 0 ; m < 8 ; ++m )
			{
			  if ( cent_cent_row + di[m] >= 0 &&  
				   cent_cent_row + di[m] < field_size && 
				   cent_cent_column + dj[m] >= 0 && 
				   cent_cent_column + dj[m] < field_size &&
				   field_data[cent_cent_row + di[m]][cent_cent_column + dj[m]] == '*' )
				{
				  num_mines_neighborhood++;
				}
			}
		  field_display[cent_cent_row][cent_cent_column] = (num_mines_neighborhood + '0');
		  if ( num_mines_neighborhood == 0 
			   && field_check[cent_cent_row][cent_cent_column] == '_')
			{
			  field_check[cent_cent_row][cent_cent_column] = '@';
			  open_neighborhood(cent_cent_row, cent_cent_column);
			}
		}
	}
}

