# for FTP EOF
.ONESHELL:

# zic lib folder
ZIC_LIB=../../zic
ZIC_APP_CORE_LIB=../../zicAppCore

# default
SDL_CONFIG=sdl2-config
CC=g++ -o zicPureData
ZIC_TARGET=0
#ZIC_SDL_MIXER=-l SDL2_mixer -DZIC_SDL_MIXER=1
LV2=`pkg-config --libs lilv-0` -I/usr/include/lilv-0

RG351_IP=192.168.1.111

#rg351
# https://github.com/christianhaitian/arkos/wiki/Building
# or https://github.com/mica-angeli/retrooz_dev_docker

linux: build run

rg351:
	make rg351Chroot rg351SCP rg351Log
	
rg351Chroot:
	sudo mount --bind /home/alex/Music/zic /mnt/data/arm64/home/alex/zic
	sudo chroot /mnt/data/arm64/ /bin/bash -c "cd /home/alex/zic/zicPureData/linuxSDL && make rg351Chrooted"

# or manualy: 
# sudo chroot /mnt/data/arm64/
# and then run `make rg351Chrooted` in the chroot

rg351Chrooted:
	make CC="g++ -o zicPureDataRG351" build

rg351SCP:
	sshpass -p "ark" scp zicPureDataRG351 ark@$(RG351_IP):/roms/ports/zic/zicPureData

rg351Kill:
	sshpass -p "ark" ssh ark@$(RG351_IP) /roms/ports/zic/kill.sh

rg351Log:
	sshpass -p "ark" ssh ark@$(RG351_IP) tail -f /roms/ports/zic/zic.log

build:
	$(CC) -Wall zicPureData.cpp -fopenmp -Wno-narrowing -I$(ZIC_LIB) -I$(ZIC_APP_CORE_LIB) `$(SDL_CONFIG) --cflags --libs` $(LV2)  -DZIC_TARGET=$(ZIC_TARGET) $(ZIC_SDL_MIXER)

# -fopenmp for soundTouch "warning: ignoring #pragma omp parallel"
# -Wno-narrowing for font.h:62:56: warning: narrowing conversion of ‘(8 * ((int)size))’ from ‘int’ to ‘Uint16’ {aka ‘short unsigned int’} inside { } [-Wnarrowing]

run:
	echo start zicPureData
	./zicPureData
