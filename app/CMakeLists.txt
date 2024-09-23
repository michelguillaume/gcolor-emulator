# Ajouter les sous-répertoires des composants
add_subdirectory(src/emulator)
add_subdirectory(src/ui)

# Créer l'exécutable pour l'application
add_executable(GColorEmulator src/main.cpp)

# Lier les bibliothèques internes (comme l'émulateur et l'UI)
# target_link_libraries(GColorEmulator emulator ui)

# Lier SDL2 si nécessaire
# target_link_libraries(GColorEmulator ${SDL2_LIBRARIES})

target_compile_options(GColorEmulator PRIVATE -Wall -Wextra)
