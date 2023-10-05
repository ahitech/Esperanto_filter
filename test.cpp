#include <Path.h>
#include <FindDirectory.h>
#include <stdio.h>

int main() {
	BPath path;
	find_directory(B_SYSTEM_NONPACKAGED_ADDONS_DIRECTORY, &path);
	
	printf("Path = %s.\n", path.Path());
	
	
	return 0;	
}
