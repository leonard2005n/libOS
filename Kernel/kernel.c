void some_function()
{

}

void main()
{
	// create a pointer to char that point to the first cell in the video
	// memory
	char *video_memory = (char *) 0xb8000;
	*video_memory = 'X';
	some_function();
}