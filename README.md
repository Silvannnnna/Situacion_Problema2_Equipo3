# Optimización de Servicios de Internet (Situación Problema 2) hola

Este proyecto en C++ resuelve cuatro problemas fundamentales de infraestructura, logística y flujo de datos para una empresa que busca implementar servicios de Internet con fibra óptica en nuevas colonias. Se aplican conceptos avanzados de Teoría de Grafos y Geometría Computacional.

El pipeline de CI/CD incluye:

- CMake + GoogleTest
- Pruebas unitarias e integración
- Cobertura mínima con lcov/gcovr
- SonarQube (análisis estático + cobertura)
- Escaneo de secretos con Gitleaks
- SAST con CodeQL
- DAST con OWASP ZAP
- Package y deploy por SSH con health check y rollback automático

---

## Equipo y División de Trabajo

### Silvanna: Infraestructura (Network Builder)
- **Responsabilidad:** Lectura del grafo y diseño del cableado de fibra óptica.
- **Algoritmo:** Árbol de Expansión Mínima (MST) — Prim o Kruskal.
- **Archivos:** `GraphParser.h/cpp`, `NetworkBuilder.h/cpp`

### Leonel: Logística y Redes (Routing & Flow)
- **Responsabilidad:** Ruta óptima de correspondencia y capacidad máxima de transmisión.
- **Algoritmos:**
  - Problema del Agente Viajero (TSP) vía Programación Dinámica (Held-Karp) o Backtracking.
  - Flujo Máximo con Ford-Fulkerson (Edmonds-Karp).
- **Archivos:** `RoutingAndFlow.h/cpp`

### Ángel: Geometría Computacional (Geo Districts)
- **Responsabilidad:** Asignación de colonias a la central más cercana y delimitación geográfica.
- **Algoritmo:** Vecino Más Cercano (Nearest Neighbor) y Diagramas de Voronoi.
- **Archivos:** `GeoDistricts.h/cpp`

---

## Estructura del Proyecto

```text
student-grade-service/
├── data/
│   └── input.txt                        # N, matrices y coordenadas
├── include/
│   ├── GraphParser.h                    # <---- (Silvanna)
│   ├── NetworkBuilder.h                 # <---- (Silvanna)
│   ├── RoutingAndFlow.h                 # <---- (Leonel)
│   └── GeoDistricts.h                   # <---- (Ángel)
├── src/
│   ├── main.cpp
│   ├── GraphParser.cpp                  # <---- (Silvanna)
│   ├── NetworkBuilder.cpp               # <---- (Silvanna)
│   ├── RoutingAndFlow.cpp               # <---- (Leonel)
│   └── GeoDistricts.cpp                 # <---- (Ángel)
├── tests/
│   ├── unit/
│   │   └── test_grade_calculator.cpp    # Pruebas unitarias (GoogleTest)
│   └── integration/
│       └── test_integration.cpp         # Pruebas de integración (GoogleTest)
├── scripts/
│   ├── check_coverage.sh
│   ├── deploy.sh
│   ├── post_deploy_healthcheck.sh
│   └── rollback.sh
├── deploy/
│   └── student-grade-service.service    # Systemd unit
├── .github/workflows/
│   └── ci-cd.yml
├── CMakeLists.txt
├── CMakePresets.json
├── sonar-project.properties
└── README.md
```

---

## Formato del archivo de entrada (`data/input.txt`)

```
N
<matriz N×N de distancias>
<matriz N×N de capacidades>
<N coordenadas (x,y)>
```

Ejemplo con N = 4 (archivo actual en `data/input.txt`):

```
4
 0 16 45 32
16  0 18 21
45 18  0  7
32 21  7  0
 0 48 12 18
52  0 42 32
18 46  0 56
24 36 52  0
(200,500)
(300,100)
(450,150)
(520,480)
```

---

## Salida esperada

```
1. Forma de cablear las colonias:
(A, B)
(B, C)
...

2. Ruta de correspondencia:
A -> B -> C -> D -> A

3. Flujo maximo:
<valor>

4. Central mas cercana:
<colonia> -> <central>
...
```

---

## 1. Requisitos locales

En Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install -y cmake ninja-build g++ lcov gcovr curl git
```

---

## 2. Compilar y ejecutar

### Opción A — compilación directa con g++

```bash
g++ -std=c++17 -Iinclude \
  src/main.cpp \
  src/GraphParser.cpp \
  src/NetworkBuilder.cpp \
  src/RoutingAndFlow.cpp \
  src/GeoDistricts.cpp \
  -o proyecto_sit2

./proyecto_sit2
```

### Opción B — con CMake (necesario para pruebas y SonarQube)

```bash
cmake --preset default
cmake --build --preset default --parallel
./build/proyecto_sit2
```

---

## 3. Ejecutar pruebas

```bash
ctest --test-dir build --output-on-failure
```

Solo unitarias:

```bash
ctest --test-dir build -R '^unit\.' --output-on-failure
```

Solo integración:

```bash
ctest --test-dir build -R '^integration\.' --output-on-failure
```

---

## 4. Generar cobertura local

```bash
cmake --preset default \
  -DCMAKE_CXX_FLAGS="--coverage" \
  -DCMAKE_EXE_LINKER_FLAGS="--coverage"
cmake --build --preset default --parallel
ctest --test-dir build --output-on-failure

mkdir -p build/coverage

lcov --capture --directory build \
  --output-file build/coverage/coverage.info

lcov --remove build/coverage/coverage.info \
  '/usr/*' '*/_deps/*' '*/tests/*' \
  --output-file build/coverage/coverage.filtered.info

./scripts/check_coverage.sh build/coverage/coverage.filtered.info
```

---

## 5. SonarQube

### 5.1 Requisitos

- Java 17 o superior
- Docker (para levantar SonarQube localmente)
- `sonar-scanner` CLI

Verificar Java:

```bash
java -version
```

### 5.2 Levantar SonarQube con Docker

```bash
docker run -d --name sonarqube -p 9000:9000 sonarqube:lts-community
```

Esperar ~1 minuto y abrir `http://localhost:9000`.  
Usuario por defecto: `admin` / `admin` — el sistema pedirá cambiar la contraseña en el primer acceso.

### 5.3 Crear un token de autenticación

1. Ir a `My Account > Security > Generate Tokens`.
2. Elegir tipo **User Token**, asignar un nombre y generar.
3. Copiar el token generado (no se volverá a mostrar).

### 5.4 Instalar sonar-scanner CLI

```bash
wget https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-6.2.1-linux-x64.zip
unzip sonar-scanner-cli-6.2.1-linux-x64.zip
export PATH="$PWD/sonar-scanner-6.2.1-linux-x64/bin:$PATH"
```

Verificar:

```bash
sonar-scanner --version
```

### 5.5 Generar `compile_commands.json` (requerido para C++)

SonarQube analiza C++ a través del archivo `compile_commands.json` que genera CMake con la flag `CMAKE_EXPORT_COMPILE_COMMANDS`:

```bash
cmake -S . -B build \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
```

Verificar que el archivo existe:

```bash
ls build/compile_commands.json
```

### 5.6 Configuración en `sonar-project.properties`

El archivo ya está configurado en la raíz del proyecto:

```properties
sonar.projectKey=grupo3Prueba
sonar.projectName=grupo3Prueba
sonar.sourceEncoding=UTF-8

sonar.sources=src,include
sonar.tests=tests

sonar.cfamily.compile-commands=build/compile_commands.json

sonar.exclusions=build/**
sonar.test.inclusions=tests/**/*.cpp
```

### 5.7 Ejecutar el escáner localmente

```bash
sonar-scanner \
  -Dsonar.host.url=http://localhost:9000 \
  -Dsonar.token=<tu-token-aqui>
```

Al terminar, los resultados estarán en `http://localhost:9000/dashboard?id=grupo3Prueba`.

### 5.8 Interpretar los resultados

| Métrica | Qué significa |
|---|---|
| **Bugs** | Errores de lógica detectados estáticamente |
| **Vulnerabilities** | Problemas de seguridad en el código |
| **Security Hotspots** | Código sensible que requiere revisión manual |
| **Code Smells** | Deuda técnica y oportunidades de mejora |
| **Coverage** | Porcentaje de líneas cubiertas por las pruebas |
| **Duplications** | Bloques de código duplicado |

Un proyecto pasa el **Quality Gate** por defecto cuando:
- No tiene bugs, vulnerabilidades ni hotspots nuevos.
- La cobertura de código nuevo es ≥ 80 %.
- La duplicación de código nuevo es < 3 %.

### 5.9 Vía CI/CD (automático)

El job `sonarqube` en `.github/workflows/ci-cd.yml` se ejecuta automáticamente en cada push a `main`, reutilizando el `compile_commands.json` y el reporte de cobertura generados por el job `build-test`. Solo requiere configurar los secrets descritos en la sección 7.

---

## 6. ¿Qué hace el pipeline?

### `secret-scan`
Busca contraseñas, tokens y llaves filtradas con Gitleaks.

### `codeql`
Realiza análisis estático de seguridad y calidad sobre C++ con CodeQL.

### `build-test`
Compila con CMake + Ninja, corre pruebas unitarias e integración, genera cobertura con lcov/gcovr y exige el umbral mínimo de cobertura (80 %).

### `sonarqube`
Envía el análisis al servidor de SonarQube. Si los secrets no están configurados, el job termina sin error para no bloquear el pipeline.

### `dast-zap`
Lanza OWASP ZAP sobre el servicio HTTP para una revisión dinámica básica de seguridad.

### `package`
Empaqueta el binario, scripts de despliegue y el unit de systemd en un archivo `.tar.gz`.

### `deploy`
Despliega por SSH en el servidor de producción, verifica el endpoint `/health` y ejecuta rollback automático si el servicio falla.

---

## 7. Secrets requeridos en GitHub Actions

Configurar en `Settings > Secrets and variables > Actions`:

| Secret | Descripción |
|---|---|
| `SONAR_HOST_URL` | URL del servidor SonarQube (ej. `https://sonar.mi-org.com`) |
| `SONAR_TOKEN` | Token de autenticación generado en SonarQube |
| `DEPLOY_HOST` | IP o hostname del servidor de producción |
| `DEPLOY_SSH_KEY` | Llave SSH privada para el usuario `ubuntu` |

---

## 8. Instalación del servicio en Linux

Copiar el archivo de systemd al servidor:

```bash
sudo cp deploy/student-grade-service.service /etc/systemd/system/
```

Habilitar e iniciar:

```bash
sudo systemctl daemon-reload
sudo systemctl enable student-grade-service
sudo systemctl start student-grade-service
sudo systemctl status student-grade-service
```

---

## 9. Rollback automático

El script `scripts/deploy.sh` guarda la versión anterior en:

```
/opt/student-grade-service-backups/<timestamp>
```

Si el health check falla tras el deploy, el workflow ejecuta `scripts/rollback.sh`, restaura el último respaldo y reinicia el servicio. El script verifica `/health` nuevamente para confirmar que el servicio quedó estable.
