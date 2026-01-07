# README

## Sistema Automático de Mezcla de Resina, Catalizador y Limpieza con Thinner

Este proyecto implementa un sistema automático basado en Arduino para el control del proceso de dosificación de resina y catalizador, mezclado, vertido y limpieza automática con thinner. El sistema está diseñado para reducir errores humanos y mantener consistencia en el proceso de fabricación.

---

## Descripción general

El sistema funciona mediante un ciclo automático que se inicia al activar un switch físico. Una vez iniciado, el programa calcula los volúmenes necesarios de resina y catalizador según la cantidad de piezas a fabricar y la temperatura ambiente. Luego controla bombas y un motor de mezclado siguiendo tiempos previamente calculados. Al final del proceso, el sistema realiza una limpieza automática utilizando thinner.

Cuando el ciclo termina, el sistema queda en reposo esperando una nueva activación del switch.

---

## Componentes controlados

* Bomba 1: Resina
* Bomba 2: Catalizador
* Bomba 3: Thinner (limpieza)
* Motor de mezclado
* Sensor de temperatura (comunicación SPI)
* Switch de inicio del ciclo

---

## Configuración de pines

* Pin 2  → Motor de mezclado
* Pin 4  → Switch de inicio
* Pin 6  → Bomba de resina
* Pin 8  → Bomba de catalizador
* Pin 10 → Bomba de thinner / CS del sensor SPI

> Nota: El pin 10 se utiliza tanto como CS del bus SPI como para la bomba de thinner. Se recomienda usar relés o transistores para aislar correctamente las cargas.

---

## Parámetros configurables

Antes de cargar el programa al Arduino, el usuario debe configurar manualmente ciertos valores en el código.

### Cantidad de piezas

La cantidad de piezas **se define manualmente en el código**, no mediante botones ni el switch.

En la parte superior del programa se encuentra la variable:

```cpp
int Cantidad;
```

El usuario debe asignarle un valor, por ejemplo:

```cpp
Cantidad = 3;
```

Este valor se usa para calcular automáticamente el volumen de resina, la cantidad de catalizador y los tiempos de bombeo.

### Tiempo de limpieza

El tiempo de limpieza con el motor de mezclado se define con:

```cpp
int TiempoLimpieza = 300000; // en milisegundos
```

Este valor puede ajustarse según las necesidades del sistema.

---

## Funcionamiento del ciclo

Cuando el switch se coloca en estado HIGH, el sistema ejecuta el siguiente flujo:

1. Calcula el volumen total de resina según la cantidad de piezas.
2. Mide la temperatura ambiente usando el sensor.
3. Define automáticamente el porcentaje de catalizador y el tiempo de mezclado.
4. Calcula el volumen de catalizador necesario.
5. Convierte los volúmenes de resina, catalizador y thinner a tiempos de bombeo.
6. Activa la bomba de resina durante el tiempo calculado.
7. Activa la bomba de catalizador durante su tiempo correspondiente.
8. Enciende el motor de mezclado por el tiempo definido según la temperatura.
9. Espera 5 minutos para permitir el vertido manual de la mezcla.
10. Activa la bomba de thinner para la limpieza.
11. Enciende nuevamente el motor de mezclado para realizar la autolimpieza del sistema.
12. Finaliza el ciclo y queda en reposo.

---

## Lógica de control por temperatura

El sistema ajusta automáticamente el proceso según la temperatura medida:

* Menor a 20 °C → 1.90 % de catalizador, 150 s de mezclado
* 20 a 24 °C    → 1.80 % de catalizador, 130 s de mezclado
* 24 a 27 °C    → 1.70 % de catalizador, 120 s de mezclado
* 27 a 30 °C    → 1.65 % de catalizador, 100 s de mezclado
* 30 a 35 °C    → 1.55 % de catalizador, 80 s de mezclado

Si la temperatura está fuera de este rango, el porcentaje se establece en 0 como medida de seguridad.

---

## Seguridad y recomendaciones

* No operar el sistema sin líquidos en las bombas.
* No activar el sistema sin la cantidad de piezas configurada.
* Usar equipo de protección personal.
* No tocar partes móviles durante el mezclado o la limpieza.
* Probar el sistema con volúmenes pequeños antes de uso continuo.

---

## Estado del sistema

Cuando el switch está apagado, el sistema permanece inactivo. El ciclo solo se ejecuta cuando el switch se encuentra en HIGH. Una vez iniciado, el ciclo se completa usando retardos (`delay`).

---

## Fin

Este proyecto está pensado como una solución automática para procesos de mezcla controlados, permitiendo futuras mejoras como interfaz de usuario, sensores adicionales o control sin bloqueos.
