# move to cmake magic

# This scripts will update each parameter file with any missing parameters
# filled in with their default values.

make update_prm_files

for f in *.prm; do
	./update_prm_files $f
done
