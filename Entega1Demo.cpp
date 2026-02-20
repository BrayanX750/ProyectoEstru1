
#include <iostream>
#include "FileSystem.h"

static void attempt(bool ok, const std::string& action, const std::string& err) {
    if (ok) {
        std::cout << "✔ " << action << "\n";
    } else {
        std::cout << "✖ " << action << " -> " << err << "\n";
    }
}

void runEntrega1Demo() {

    FileSystem fs;
    std::string err;

    attempt(fs.createDirectory("/", "Docs", err), "Crear /Docs", err);
    attempt(fs.createDirectory("/", "Images", err), "Crear /Images", err);
    attempt(fs.createDirectory("/Docs", "Projects", err), "Crear /Docs/Projects", err);

    attempt(fs.createFile("/Docs", "readme.txt", err), "Crear /Docs/readme.txt", err);
    attempt(fs.createFile("/Docs/Projects", "plan.pdf", err), "Crear /Docs/Projects/plan.pdf", err);

    // Duplicado (debe fallar)
    attempt(fs.createFile("/Docs", "readme.txt", err), "Intentar duplicado /Docs/readme.txt", err);

    // Ruta inválida (debe fallar)
    attempt(fs.createFile("/DoesNotExist", "x.txt", err), "Intentar crear archivo en ruta inválida", err);

    // Nombre inválido (debe fallar)
    attempt(fs.createDirectory("/Docs", "bad/name", err), "Intentar crear directorio con nombre inválido", err);

    std::cout << "\n--- ARBOL ACTUAL ---\n";
    fs.printTree();


}
