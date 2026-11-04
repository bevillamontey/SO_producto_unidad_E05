import math

def simular_sistema_memoria():
    # 1. Configuración de parámetros iniciales
    TAMANO_PAGINA = 4096  # 4 KB por página (Común en SO reales)
    CANTIDAD_MARCOS = 8   # La memoria física tendrá 8 marcos (0 a 7)
    MAX_PAGINAS_PROCESO = 6 # El proceso tiene 6 páginas virtuales en su espacio lógico (0 a 5)

    # 2. Simulación de la Tabla de Páginas (Asignación)
    # Clave: Número de Página Virtual (VPN)
    # Valor: Número de Marco Físico (PFN) o None si no está en RAM
    tabla_paginas = {
        0: 2,     # Página 0 asignada al Marco 2
        1: 5,     # Página 1 asignada al Marco 5
        2: 0,     # Página 2 asignada al Marco 0
        3: None,  # Página 3 es válida, pero no está asignada en RAM (Fallo de página)
        4: 7,     # Página 4 asignada al Marco 7
        5: None   # Página 5 es válida, pero no está asignada en RAM (Fallo de página)
    }

    # Mostrar la configuración del sistema
    print("=" * 60)
    print("         SIMULACIÓN DE SISTEMA DE MEMORIA PAGINADA")
    print("=" * 60)
    print(f"[*] Tamaño de cada página / marco: {TAMANO_PAGINA} bytes")
    print(f"[*] Cantidad de marcos físicos en RAM: {CANTIDAD_MARCOS}")
    print(f"[*] Espacio lógico máximo del proceso: {MAX_PAGINAS_PROCESO} páginas")
    print(f"[*] Rango de direcciones lógicas válidas: 0 a {MAX_PAGINAS_PROCESO * TAMANO_PAGINA - 1} bytes\n")

    # Mostrar la tabla de páginas actual de manera estructurada
    print("--- TABLA DE PÁGINAS DEL PROCESO ---")
    print(f"{'Página Virtual':<16} | {'Marco Físico':<14} | {'Estado en RAM':<15}")
    print("-" * 52)
    for pag, marco in tabla_paginas.items():
        if marco is not None:
            estado = "Asignada"
            marco_str = str(marco)
        else:
            estado = "No asignada (Page Fault)"
            marco_str = "-"
        print(f"{pag:<16} | {marco_str:<14} | {estado:<15}")
    print("-" * 52)

    # 3. Bucle interactivo para traducir direcciones lógicas
    print("\nIntroduce una dirección lógica en bytes para traducirla a física.")
    print("(Escribe 'salir' para terminar el programa)\n")

    while True:
        entrada = input("Dirección Lógica > ").strip()
        
        if entrada.lower() == 'salir':
            print("Simulación finalizada.")
            break
            
        if not entrada.isdigit():
            print("* Error: Por favor, introduce un número entero válido.\n")
            continue

        direccion_logica = int(entrada)

        # Algoritmo de traducción de direcciones (MMU)
        num_pagina = direccion_logica // TAMANO_PAGINA
        desplazamiento = direccion_logica % TAMANO_PAGINA

        print(f"🔍 Análisis: Dirección Lógica dividida en -> Página: {num_pagina}, Desplazamiento (Offset): {desplazamiento}")

        # Caso 1: Identificar dirección inválida (Fuera del espacio de direcciones del proceso)
        if num_pagina not in tabla_paginas:
            print(f"* ERROR: Dirección Inválida. La página {num_pagina} no pertenece al espacio lógico del proceso.")
            print(f"   (Límite excedido. El proceso solo tiene {MAX_PAGINAS_PROCESO} páginas asignadas de la 0 a la {MAX_PAGINAS_PROCESO - 1}).\n")
            continue

        # Caso 2: Identificar página no asignada en memoria física (Fallo de página / Page Fault)
        marco = tabla_paginas[num_pagina]
        if marco is None:
            print(f"* ERROR: Página No Asignada. La página {num_pagina} es válida, pero se produjo un 'Fallo de Página' (Page Fault).")
            print("   La página debe ser traída desde el almacenamiento secundario (disco) a la RAM.\n")
            continue

        # Caso 3: Éxito en la asignación y cálculo de la Dirección Física
        direccion_fisica = (marco * TAMANO_PAGINA) + desplazamiento
        print(f"* ÉXITO: Dirección Física Resultante = {direccion_fisica}")
        print(f"   Fórmula utilizada: (Marco {marco} * Tamaño Página {TAMANO_PAGINA}) + Desplazamiento {desplazamiento}\n")

if __name__ == "__main__":
    simular_sistema_memoria()