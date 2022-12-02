CC = gcc
CFLAGS = -Wall -g
FLAGS = -Wall -g
LIBS = -lsqlite3

LAUNCHER_SRV = serveur_app.o
LAUNCHER_CLT = client_app.o
BUILD_PATH = build
DB_PATH = database

SRC_SRV_PATH = src/serveur
BUILD_SRV_PATH = $(BUILD_PATH)/serveur

SRC_CLT_PATH = src/client
BUILD_CLT_PATH = $(BUILD_PATH)/client

RM = rm

TARGET_SRV = $(notdir $(basename $(shell find $(SRC_SRV_PATH) -name *.c)))
TARGET_CLT = $(notdir $(basename $(shell find $(SRC_CLT_PATH) -name *.c)))

LIBRARIES_SRV = $(shell find $(SRC_SRV_PATH) -name *.h)
LIBRARIES_CLT = $(shell find $(SRC_CLT_PATH) -name *.h)

BUILD_SRV = $(addprefix $(BUILD_SRV_PATH)/,$(TARGET_SRV))
BUILD_CLT = $(addprefix $(BUILD_CLT_PATH)/,$(TARGET_CLT))

app: $(BUILD_PATH) serveur_app client_app

$(BUILD_PATH):
	@echo Creating folders...
	@mkdir $(BUILD_PATH)
	@mkdir $(BUILD_SRV_PATH)
	@mkdir $(BUILD_CLT_PATH)

serveur_app: $(BUILD_SRV) 
	$(CC) $(CFLAGS) -o $(LAUNCHER_SRV) $(BUILD_SRV) $(LIBS)

client_app: $(BUILD_CLT) 
	$(CC) $(CFLAGS) -o $(LAUNCHER_CLT) $(BUILD_CLT) $(LIBS)
	
$(BUILD_SRV_PATH)/%: $(SRC_SRV_PATH)/%.c $(LIBRARIES_SRV)
	$(CC) $(CFLAGS) -c -o $@ $(SRC_SRV_PATH)/$(notdir $@).c

$(BUILD_CLT_PATH)/%: $(SRC_CLT_PATH)/%.c $(LIBRARIES_CLT)
	$(CC) $(CFLAGS) -c -o $@ $(SRC_CLT_PATH)/$(notdir $@).c

# CLEANING FOLDER
clear:
	@echo Cleaning...
	@$(RM) -r $(BUILD_PATH)
	@$(RM) $(DB_PATH)/*.db
	@$(RM) $(LAUNCHER_SRV)
	@$(RM) $(LAUNCHER_CLT)

	