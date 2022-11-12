extern "C" {
	int main(int argc, char **argc, char **env);

	int _start(int argc, char **argc, char **env)
	{
		int result = main(argc, argc, env);
		exit(result);
		return 1;
	}
}