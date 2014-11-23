#!/usr/bin/bash

subfolder=$1

cd $subfolder
cat Makefile.base > Makefile
printf "\n" >> Makefile
for folder in `find . -path ./obj -prune -o -type d -print`; do
	mkdir -p obj/$folder
done

sources=`find . -type f -name \*.c`
objects=""
for file in $sources; do
	objects+="obj/${file%?}o "
done
printf "obj/\$(TARGET): $objects\n" >> Makefile
printf '\t$(CC) $(LDFLAGS) -o $@ $^\n\n' >> Makefile 

for file in $sources; do
	deps=`grep '#include "' $file | cut -d ' ' -f 2 | sed 's/^"\(.*\)"$/..\/\1/g' | tr '\n' ' '`
	fileDotO="obj/${file%?}o"
	printf "$fileDotO: $file $deps\n" >> Makefile
	printf '\t$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<\n\n' >> Makefile
done

cd ..
