#/bin/bash
#TODO: dispatch in several files and run in parallel ?!

#initialize main.c
printf '#include <stdlib.h>\n' > main.c
printf '\n' >> main.c
printf 'int main(int argc, char** argv)\n' >> main.c
printf '{\n' >> main.c

#add functions
for file in `find . -type f -name \*.c ! -name main.c`; do
	printf "\t//file $file :\n" >> main.c
	functions=`grep '^ *void t_' $file | sed 's/^ *void \([^(]*\).*/\t\1();/g'`
	printf "$functions" >> main.c
	printf '\n\n' >> main.c
done

#finalize main.c
printf '\treturn 0;\n' >> main.c
printf '}\n' >> main.c
