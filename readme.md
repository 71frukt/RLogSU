# Логгер эээ

## Зависимости

| Зависимость           | Минимальная версия    | Назначение                                    |
|-----------------------|-----------------------|-----------------------------------------------|
| **CMake**             | 3.12                  | Сборка проекта и зависимостей                 |
| **libfmt-dev**        | 8.1.1                 | Парсинг форматных строк                       |
| **graphviz**          | 2.43.0                | Графический дамп                              |
| **GCC / Clang**       | GCC 10 / Clang 11     | Компиляция C++20 кода                         |

### Установка зависимомтей на Ubuntu
* libfmt
```zsh
sudo apt install libfmt-dev
```

* graphviz
```zsh
sudo apt install graphviz
```

## Пример работы

```C++
#include <stdexcept>
#include <string>
#include <vector>

#include "RLogSU/graph.hpp"
#include "RLogSU/logger.hpp"

int main() try {
    RLSU_SET_LOGSPACE("build/custom_logspace")

    RLSU_INFO    ("'{}'",   "info"    );
    RLSU_MESSAGE ("'{}'",   "message" );
    RLSU_LOG     ("'{}'\n", "log"     );
    RLSU_WARNING ("'{}'",   "warning" );
    RLSU_ERROR   ("'{}'",   "error"   );

    std::vector arr = {11, 22, 33, 44, 55, 66, 77, 88, 99};
    
    RLSU::Graphics::Graph lingraph;
    lingraph.AddNode(&arr[0], std::to_string(arr[0]));
    for (size_t i = 1; i < arr.size(); ++i)
    {
        lingraph.AddNode(&arr[i], std::to_string(arr[i]));
        lingraph.AddEdge(&arr[i], &arr[i - 1]);
        lingraph.AddEdge(&arr[i], &arr[arr.size() - i]);
    }
    
    RLSU_DUMP(lingraph.LogGraph(), "lingraph");

    RLSU_THROW<std::runtime_error>("exception\?\?\?((((");
}

catch (std::runtime_error& e)
{
    RLSU_LOG_RUNTIME_ERR(e);
}
```

#### Консоль
<image src="readme_images/exmpl1.png" alt="3s" width=60%>

#### Логфайл
<image src="readme_images/exmpl2.png" alt="3s" width=60%>
