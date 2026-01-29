# Options du compilateur
CFLAGS = -Wall -Wextra -O2 -std=c11 -Iinclude -Wno-deprecated-declarations

# Options du linker
LDFLAGS = -lflint -lgmp -lmpfr

# Dossiers
SRC_DIR = src
INC_DIR = include
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin

# Fichiers sources
SOURCES = ell_curve.c tors_ring.c ell_point.c list.c schoof.c
OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Fichier de test (JE VAIS SUREMENT RAJOUTER UN TEST DE MESURE DU TEMPS D'EXECUTION)
TEST_SOURCES = test_compare.c
TEST_OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(TEST_SOURCES))

# Exécutables
TEST_BIN = $(BIN_DIR)/test_schoof

# Paramètres de tests par défaut
NUM_TRIALS ?= 5
MIN_BITS ?= 8
MAX_BITS ?= 32

# Condition MIN_BITS supérieur à 4
ifeq ($(shell test $(MIN_BITS) -lt 4; echo $$?),0)
    $(error ⚠️  MIN_BITS doit valoir au moins 4)
endif

# Condition MAX_BITS >= MIN_BITS
ifeq ($(shell test $(MAX_BITS) -lt $(MIN_BITS); echo $$?),0)
    $(error ⚠️  MAX_BITS doit être supérieur à MIN_BITS)
endif

# Flags de test
TEST_FLAGS = -DNUM_TRIALS=$(NUM_TRIALS) -DMIN_BITS=$(MIN_BITS) -DMAX_BITS=$(MAX_BITS)

# Code couleur ANSI
GREEN = \033[0;32m
BLUE = \033[0;34m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m

# Commande par défaut
.PHONY: all
all: info $(TEST_BIN)
	@echo "$(GREEN)✓ Compilation terminée avec succès !$(NC)"

# Affichage des paramètres
.PHONY: info
info:
	@echo "$(BLUE)=========================================$(NC)"
	@echo "$(BLUE)  Compilation de l'algorithme de Schoof$(NC)"
	@echo "$(BLUE)=========================================$(NC)"
	@echo "Paramètres de test :"
	@echo "  NUM_TRIALS = $(YELLOW)$(NUM_TRIALS)$(NC)"
	@echo "  MIN_BITS   = $(YELLOW)$(MIN_BITS)$(NC)"
	@echo "  MAX_BITS   = $(YELLOW)$(MAX_BITS)$(NC)"
	@echo ""

# Créer les dossiers nécessaires
$(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

# Compilation des fichiers objets sources (ON POURRAIT LES RENDRE PLUS COMPACTS)
$(OBJ_DIR)/ell_curve.o: $(SRC_DIR)/ell_curve.c $(INC_DIR)/ell_curve.h | $(OBJ_DIR)
	@echo "$(BLUE)Compilation de $<...$(NC)"
	@gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/tors_ring.o: $(SRC_DIR)/tors_ring.c $(INC_DIR)/tors_ring.h $(INC_DIR)/ell_curve.h | $(OBJ_DIR)
	@echo "$(BLUE)Compilation de $<...$(NC)"
	@gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/ell_point.o: $(SRC_DIR)/ell_point.c $(INC_DIR)/ell_point.h $(INC_DIR)/tors_ring.h $(INC_DIR)/ell_curve.h | $(OBJ_DIR)
	@echo "$(BLUE)Compilation de $<...$(NC)"
	@gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/list.o: $(SRC_DIR)/list.c $(INC_DIR)/list.h | $(OBJ_DIR)
	@echo "$(BLUE)Compilation de $<...$(NC)"
	@gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/schoof.o: $(SRC_DIR)/schoof.c $(INC_DIR)/schoof.h $(INC_DIR)/tors_ring.h $(INC_DIR)/ell_curve.h $(INC_DIR)/ell_point.h $(INC_DIR)/list.h | $(OBJ_DIR)
	@echo "$(BLUE)Compilation de $<...$(NC)"
	@gcc $(CFLAGS) -c $< -o $@

# Compilation des fichiers de test
$(OBJ_DIR)/test_compare.o: $(TEST_DIR)/test_compare.c $(TEST_DIR)/test_compare.h $(INC_DIR)/ell_curve.h $(INC_DIR)/schoof.h | $(OBJ_DIR)
	@echo "$(BLUE)Compilation de $< avec paramètres de test...$(NC)"
	@gcc $(CFLAGS) $(TEST_FLAGS) -c $< -o $@

# Création de l'exécutable de test
$(TEST_BIN): $(OBJECTS) $(TEST_OBJECTS) | $(BIN_DIR)
	@echo "$(BLUE)Création de l'exécutable de test...$(NC)"
	@gcc $(OBJECTS) $(TEST_OBJECTS) -o $@ $(LDFLAGS)

# Forcer la recompilation des tests quand les paramètres changent
.PHONY: force-test-rebuild
force-test-rebuild:
	@rm -f $(TEST_OBJECTS)

# Tests rapides
.PHONY: test-quick
test-quick:
	@$(MAKE) test NUM_TRIALS=3 MIN_BITS=8 MAX_BITS=16

# Exécution des tests
.PHONY: test
test: force-test-rebuild $(TEST_BIN)
	@echo "$(BLUE)=======================$(NC)"
	@echo "$(BLUE)  Éxécution des tests$(NC)"
	@echo "$(BLUE)=======================$(NC)"
	@echo "Paramètres :"
	@echo "  NUM_TRIALS = $(YELLOW)$(NUM_TRIALS)$(NC)"
	@echo "  MIN_BITS   = $(YELLOW)$(MIN_BITS)$(NC)"
	@echo "  MAX_BITS   = $(YELLOW)$(MAX_BITS)$(NC)"
	@echo ""
	@$(TEST_BIN)

# Nettoyage
.PHONY: clean
clean:
	@echo "$(YELLOW)Nettoyage des fichiers objets et exécutables...$(NC)"
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "$(GREEN)Nettoyage terminé$(NC)"

# Affichage de l'aide
.PHONY: help
help:
	@echo "$(BLUE)================================$(NC)"
	@echo "$(BLUE)  Algorithme de Schoof - Aide$(NC)"
	@echo "$(BLUE)================================$(NC)"
	@echo ""
	@echo "$(GREEN)Commandes disponibles :$(NC)"
	@echo "  $(YELLOW)make $(NC)ou $(YELLOW)make all$(NC)      - Compile le projet"
	@echo "  $(YELLOW)make test-quick$(NC)       - Tests rapides (3 essais, 8-16 bits)"
	@echo "  $(YELLOW)make test$(NC)             - Exécute les tests (5 essais, 8-32 bits)"
	@echo "  $(YELLOW)make clean$(NC)            - Supprime les fichiers objets et exécutables"
	@echo "  $(YELLOW)make help$(NC)             - Affiche cette aide"
	@echo ""
	@echo "$(GREEN)Paramètres personnalisables :$(NC)"
	@echo "  $(YELLOW)NUM_TRIALS$(NC)  - Nombre d'essais par taille (défaut: 5)"
	@echo "  $(YELLOW)MIN_BITS$(NC)    - Taille minimale en bits (défaut: 8)"
	@echo "  $(YELLOW)MAX_BITS$(NC)    - Taille maximale en bits (défaut: 32)"
	@echo ""
	@echo "$(GREEN)Exemple :$(NC)"
	@echo "  $(YELLOW)make test NUM_TRIALS=2 MIN_BITS=16 MAX_BITS=32$(NC)"
	@echo ""
	@echo "$(RED)Note: MIN_BITS doit valoir au moins 4$(NC)"