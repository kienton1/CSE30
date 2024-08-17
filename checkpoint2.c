int main()
{
	char *park = "Denali National Park";
	char *s1;
	char *s3;
	s1 = park;
	char **s2;
	char c;
	s2 = &s1;
	*s2 = s1 + 2;
	c = **s2;
	s3 = s1++;
	return 0;
}
