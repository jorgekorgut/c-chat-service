CC = gcc
CFLAGS = -Wall -g
FLAGS = -Wall -g

LAUNCHER_SRV = serveur_app.o
LAUNCHER_CLT = client_app.o

SRC_SRV_PATH = src/serveur
BUILD_SRV_PATH = build/serveur

SRC_CLT_PATH = src/client
BUILD_CLT_PATH = build/client

RM = rm

TARGET_SRV = $(notdir $(basename $(shell find $(SRC_SRV_PATH) -name *.c)))
TARGET_CLT = $(notdir $(basename $(shell find $(SRC_CLT_PATH) -name *.c)))

LIBRARIES_SRV = $(shell find $(SRC_SRV_PATH) -name *.h)
LIBRARIES_CLT = $(shell find $(SRC_CLT_PATH) -name *.h)

BUILD_SRV = $(addprefix $(BUILD_SRV_PATH)/,$(TARGET_SRV))
BUILD_CLT = $(addprefix $(BUILD_CLT_PATH)/,$(TARGET_CLT))

app: serveur_app client_app
	
serveur_app: $(BUILD_SRV) 
	$(CC) $(CFLAGS) -o $(LAUNCHER_SRV) $(BUILD_SRV)

client_app: $(BUILD_CLT) 
	$(CC) $(CFLAGS) -o $(LAUNCHER_CLT) $(BUILD_CLT)

$(BUILD_SRV_PATH)/%: $(SRC_SRV_PATH)/%.c $(LIBRARIES_SRV)
	$(CC) $(CFLAGS) -c -o $@ $(SRC_SRV_PATH)/$(notdir $@).c

$(BUILD_CLT_PATH)/%: $(SRC_CLT_PATH)/%.c $(LIBRARIES_CLT)
	$(CC) $(CFLAGS) -c -o $@ $(SRC_CLT_PATH)/$(notdir $@).c

# CLEANING FOLDER
clear:
	@echo Cleaning...
	@$(RM) $(BUILD_SRV_PATH)/*
	@$(RM) $(BUILD_CLT_PATH)/.*
	@$(RM) ./*.o

	