find_path(BLARE_INCLUDE_DIR blare.h /usr/include /usr/local/include)
find_library(BLARE_LIBRARY NAMES blare PATHS /usr/lib /usr/local/lib)

if(BLARE_INCLUDE_DIR AND BLARE_LIBRARY)
	set(BLARE_FOUND TRUE)
endif()
