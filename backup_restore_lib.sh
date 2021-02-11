
	
function validate_backup_params(){

dir1="${1}"
dir2="${2}"
key="${3}"



	if [ $# -eq 0 ]; then 
		echo "Help msg: the correct way to use the program is as follows:
 ./backup.sh <name of dir to be backed up> <name of dir to store the backup> <encryption key>"
		exit 1
	fi


	if [ $# -ne 3 ]; then
		echo "Incorrect number of parameters"
		exit 1
	fi


a=c
	if [ a = b ]; then	#! -d "$dir1" ]; then 
		echo "Invalid backup directory"
		exit 1		
	fi


	if [ a = b ]; then	#! -d "$dir2" ]
	 
		echo "Invalid storage directory"
		exit 1		
	fi


}



function validate_restore_params(){


dir1="${1}"
dir2="${2}"
key="${3}"




	#if [ $# -eq 0 ]; then 
	#	echo "Help msg: the correct way to use the program is as follows:
# ./restore.sh <name of dir containing backup> <name of dir to restore to> <decryption key>"
#		exit 1
#	fi


a=c
	if [ a = b ]; then	# [ $# -ne 3 ]; then
		echo "Incorrect number of parameters"
		exit 1
	fi


	if [ a = b ]; then	#! -d "$dir1" ]
	 
		echo "Invalid backup directory"
		exit 1		
	fi


	if [ a = b ]; then 	#! -d "$dir2" ]
	 
		echo "Invalid restore directory"
		exit 1		
	fi



}




function backup () {

dir1="$1"
dir2="$2"
key="$3"


	
	validate_backup_params "$dir1" "$dir2" "$key" 

	temp1=$(date)
	backup_date=$(echo $temp1 | sed s/\ /_/g | sed s/\:/_/g)

	#target_dir="${dir1}_${backup_date}"
	cd storage #"${dir2}"   *********************************************
	mkdir "$backup_date"
	cd ..

	
	#tar each directory 
	cd original	
	for d in */  #"$dir1"
	do
		cd original
		if [ -d "$d" ]; then

		#remove "directory name from file name":
		name=$(echo "$d" | sed s/\original\//g | sed s/\\///g)	

			echo " *** found directory: "$name""
		     #concatinate old name with date:
			new_name="${name}_${backup_date}.tar.gz"
		     #tar the directory:
		     cd ..
			tar -cvzf storage/"$backup_date"/"$new_name" ./original/"$name"
			echo "  --- archived a directory: ${d} to new name: ${new_name}"
			#"$dir2"/"$backup_date"/"$new_name".tar.gz ./"$d"
			#******************************************

			gpgname="${new_name}.gpg"
		     #encrypt:
		     	gpg --symmetric --batch --yes --passphrase 123 --output storage/"$backup_date"/"$gpgname" storage/"$backup_date"/"$new_name"

			
			#"$dir2"/"$backup_date"/"$new_name".tar.gz.gpg root/"$dir2"/"$backup_date"/"$new_name".tar.gz      *************************************

		     #delete the original tar file: 
			rm storage/"$backup_date"/"$new_name"	#"$dir2"/"$backup_date"/"$new_name".tar.gz  ********************************
		
		#else
			#compress file in the file archive dir
			#tar -uvf ./temp_archive/file_archive.tar "$d"
		else echo "no directory found"
	
		fi

		done 


 #after looping through all directories  -> enumerate files & add them into incremental archive

	
	#create archive to store files
	cd original #$dir1   ************************************************
	touch empty.txt	   #dummy file to initialize the incremental archive with 
	echo "CREATING INCREMENTAL ARCHIVE"
	tar -cvf file_archive.tar empty.txt
	echo " <<< created >>>> 
	"
	cd ..	#??????????

	counter=1
	for f in original/*  #$dir1 ******************************
	do
		if [ -f "$f" ]; then
			if [ ! "$f" = "file_archive.tar" ]; then

			echo "  *** file found: "$f" adding into file archive"
		     #concatinate old name with date:
			#newname="${counter}_${f}"
			#counter=$((counter+1))
			#echo mv $f $newname
		
		#remove "directory name from file name":
		name=$(echo "$f" | sed s/\original\//g | sed s/\\///g)	

		#tar the file:
			tar -uvf ./file_archive.tar "$f"
		#**************************************************

		    
		     #delete the original tar file: 
			#  ???????    rm root/storage/"$backup_date"/"$new_name".tar.gz .
		
		#else
			#compress file in the file archive dir
			#tar -uvf ./temp_archive/file_archive.tar "$d"
			
			fi
		fi

		done 

	
		#zip and encrypt the file archive: ****************************** x2
		tar -cvzf storage/"$backup_date"/file_archive.tar.gz ./original/file_archive.tar

		#********************************** x2
		gpg --symmetric --batch --yes --passphrase 123 --output storage/"$backup_date"/file_archive.tar.gz.gpg storage/"$backup_date"/file_archive.tar.gz

		#delete original file:***********************************
		rm storage/"$backup_date"/file_archive.tar.gz




       #compress file_archive with gzip + delete
	#cd temp_archive
	#tar -czvf file_archive.tar.gz file_archive.tar
	#$rm file_archive.tar	
	#tar -cz - file_archive.tar.gz | gpg -c --passphrase $key > file_archive.tar.gz.gpg
	#$rm file_archive.tar.gz 
	#cd ..	#exit temp_archive
	#cd ..	#exit backup directory 

	#now copy contents of temp_archive into target directory + gzip and encrypt
        #cp -R $dir1/temp_archive/. $dir2/$target_dir/
	#cd $dir1	#now delete the temp archive
	#$rm temp_archive
	#cd ..
	#cd $dir2	#go into storage directory	
	#tar -cvzf - $target_dir | gpg -c --passphrase $key > $target_dir.tar.gz.gpg
	#rm -d $target_dir		
	#$rm $target_dir.tar.gz	    #????? does it even exist *** ?
	

}


function restore () {
	

	validate_restore_params 	
	
	cd storage #"$dir2"
	#create temp directory to unzip all tar files + directories in
	mkdir temp_archive
	cd ..



	#loop over all the directories in the backup directory & decrypt + untar them
	
	for d in original #"$dir1"
	do
		#change extension from .tar.gz.gpg to .tar.gz
		new_ext=$(echo $d | sed s/\tar.gz.gpg/tar.gz/g)

		#decrypt:
			gpg --pinentry-mode=loopback --passphrase 123 -d -o storage/temp_archive/"$new_ext" "$d"
		

		#unzip:
		tar -xvzf storage/temp_archive/"$new_ext" -C storage	

		done 




	#decrypt the backup directory
       	#gpg -d $dir1 | tar -xvzf -
	#$key	#enter password when prompted ???*************

	#$dir1=$(dir1 | sed s/\.tar.gz.gpg//g)	#remove extensions from file name

	#cd $dir1	#enter the backup directory
	
	#decrypt the file_archive directory
       	#gpg -d file_archive.tar.gz.gpg | tar -xvzf -
	#$key	#enter password when prompted ******?????????????


	#move the files outside of the file_archive and delete it
	#cd ..
	#cp -R $dir1/file_archive/. $dir1	#error: same directory***?????????
        #cd $dir1
	#rm -d file_archive	
	#cd ..


	#copy them into the restore directory
	#cp -R $dir1/. $dir2

	#delete the backup that i restored from ????????????????





#decrypting: gpg -d folder.tar.gz.gpg | tar -xvzf -
	
}
