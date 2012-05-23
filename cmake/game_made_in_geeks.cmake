

option(game_made_in_geek "build made in geek" OFF)
set(geek_made_in_geeks_games "ogre" "ois" "cegui" "fmod")
macro(geek_made_in_geeks)
	if(game_made_in_geek)
		add_subdirectory(${PROJECT_SOURCE_DIR}/game_made_in_geeks/main)
		#add_subdirectory(${PROJECT_SOURCE_DIR}/game_make_in_geeks/common_components)
	endif(game_made_in_geek)
endmacro(geek_made_in_geeks)