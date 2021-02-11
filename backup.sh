
source backup_restore_lib.sh

dir1="$1"
dir2="$2"
key="$3"


	if [ $# -eq 0 ]; then 
		echo "Help msg: the correct way to use the program is as follows:
 ./backup.sh <name of dir to be backed up> <name of dir to store the backup> <encryption key>"
		exit 1
	fi


	if [ $# -ne 3 ]; then
		echo "Incorrect number of parameters"
		exit 1
	fi


	if [ ! -d "$dir1" ]; then 
		echo "Invalid backup directory"
		exit 1		
	fi


	if [ ! -d "$dir2" ]; then
	 
		echo "Invalid storage directory"
		exit 1		
	fi




backup $dir1 $dir2 $key
