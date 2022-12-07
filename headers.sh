#! /bin/sh

# name that will be inserted at the top of each header file
# YOU MUST CHANGE THIS TO YOUR NAME BEFORE USING THIS FILE
# TURNING IN CODE WITH ANOTHER STUDENTS NAME IN IT IS A COAM OFFENSE
# - Dr. Kirby
readonly AUTHOR_NAME="Chris Barlas and Ben Elleman"

# do the mystery that the professors did for us
echo "Creating header files."
ctags -x --c-kinds=f --file-scope=no *.c | awk -f headers.awk

# create the header files from the visual symbols the professors did for us
for f in *.vs; do
    [ -f "$f" ] || break

    # get the name of the file with the .h extension
    filename="$(basename $f .vs).h"

    # notify the user we are crafting them a delicous header file
    echo -e "\tcreating $filename from $f"

    # clear the file and write the aution comment string
    echo -e "/* $AUTHOR_NAME */\n" > $filename
    
    # append everything in the visual symbol files
    cat $f >> $filename
done

date > headers
echo "Done creating headers."