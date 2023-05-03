# DBII-PROJECT-1

## Introduccion

### Objetivos del proyecto

### Descripcion del dataset
Obtener la informacion de Kaggle: 
- Titanic: ...
- Pokemón: ...

### Resultados esperados

## Tecnicas utilizadas

### Sequential File Indexing
Sequential File es una tecnica usada para mejorar el rendimiento en busqueda y acceso de data en el archivo. En un Sequential File los registros se guardan de forma ordenada (sequencial), tipicamente en memoria secundaria.

Sequential File es una simple y efectiva forma de mejorar el rendimiento de obtencion de data (Data retrieval). Sin embargo, tiene limitaciones, como la necesidad de un archivo auxiliar, y el mantenimiento de los punteros `prev` y `next` cuando se eliminan o añaden registros.
#### Insercion 
Los registros nuevos son insertados en un archivo auxiliar. Cada registro cuenta con punteros `next` y `prev` que apuntan al resto de registros en el archivo.

Se realiza una busqueda binaria O(logn) en el `DataFile` (archivo orginal ordenado). El cual retorna una posición (posición física - archivo) cercana a la que se debe insertar. Debido a que puede haber registros en el archivo auxiliar `AuxFile` y la busqueda es solamente en el `DataFile`. Por lo que, a partir de la posición cercana se busca 3 posiciones no necesariamente contiguas (begin, mid, end). Para actualizar los punteros solo se necesitan 3 posiciones, la posicion de insercion (mid), el anterior a este (begin) y el siguiente (end).

![Alt Text](/images/pointers1.png)
#### Eliminacion
Los registros a eliminar se buscan usando busqueda binaria (igual que en la Inserción) y se retoran una posición cercan en el `DataFile`. A partir de esta posicion volvemos a conseguir las 3 posiciones begin, mid y end. Se evaluan los casos (si es el primer registro, el ultimo registro, cualquier registro). Se cambia el atributo `is_deleted = true`. y se cambian los punteros del registro anterior y siguiente. (Cada registro cuenta con punteros `next` y `prev` que apuntan al resto de registros en el archivo.)


![Alt Text](/images/pointers2.png)
#### Busqueda
El registro a buscar puede o no estar en el `DataFile`. Sea el caso que sea, sabemos que el `DataFile` está ordenado y el `AuxFile` no. Entonces, se realiza una busqueda binaria en el `DataFile`, el cual retorna una posicion cercana a la que estamos buscando. En este caso, usamos los métodos `look_before()` y `look_after()` para recorrer los registros usando los punteros `next` y `prev`. Hasta encontrar los registros que cumplan con la busqueda.

![Alt Text](/images/pointers3.png)
### tecnica 2
#### Insercion
#### Eliminacion
#### Busqueda

### tecnica 3
#### Insercion
#### Eliminacion
#### Busqueda

## Resultados Experimentales
### Tablas y graficas comparativas (log scale)

## Pruebas de uso y presentacion
Adjuntar fotos, videos, etc. Mostrando la funcionalidad del programa
