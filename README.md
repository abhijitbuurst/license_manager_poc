

# license_manager_poc
git clone https://github.com/abhijitbuurst/license_manager_poc.git

cd license_manager_poc/
# --------------
Some steps required on Nalpeiron portal
1. Create a product in Nalpeiron portal
2. Download zentitle dynamic link library(.so for linux)
3. put that .so file inside repo
# -----------
Update configuration.h
1. update cust id, prod id, Xcont, yconst, zconst
2. update full path of zentitle library 

# -----------
make clean

make

./license_manager_interface
