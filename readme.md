# Описание

Игра Endless Survival представляет собой бесконечную 2д survival игру с видом
сверху. В ней вам предстоит сразиться с тысячами монстров, продержаться как
можно дольше. Усильте своего персонажа, чтобы выдержать новые, более
сложные волны врагов. Найдите Звезду Эдема в бесконечном мире, чтобы
навсегда искоренить зло

# Игровой процесс

Игрок исследует открытый мир, взаимодействует с объектами и сражается с врагами, такими как Бегун, Прыгун и Босс. Для борьбы улучшайте свое оружие, чтобы одолеть любого врага.

# Цель Игры

Главная цель игры — Найти звезду Эдема.

# Видео геймплея

[Смотреть видео геймплея](https://drive.google.com/file/d/1C4JgXZqDNDg44YZvI_N_0jWcBCTyWuN9/view?usp=sharing)


# Требования

- **CMake** (версия 3.28 или выше)
- **MinGW** (для сборки. Крайне желательно использовать MSYS2 установщик)
- **Git** (для клонирования репозитория)


# Шаги по установке и сборке

1. **Установите Git:**

   Скачайте и установите Git с [официального сайта](https://git-scm.com/download/win).

2. **Установите MinGW:**

   Скачайте и установите MSYS2 с [официального сайта](https://www.msys2.org/). Во время установки выберите:
   - `Run MSYS2 now`

   В открывшейся консоли выполните:
   ```sh
   pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
   ```
   Терминал покажет следующий вывод, представленный ниже. Нажмите enter для продолжения
   ![alt text](https://github.com/foblako/EndlessSurvivors/blob/master/Readme%20images/image-5.png)

   Затем также введите Y, чтобы продолжить установку

   Добавьте `C:\msys64\ucrt64\bin` в переменную окружения `PATH`.
   (Поиск - изменение системных переменных среды - переменные среды - Path)

3. **Установите CMake:**

   Скачайте и установите CMake с [официального сайта](https://cmake.org/download/). Во время установки выберите опцию "Add CMake to the system PATH for all users".


4. **Склонируйте репозиторий проекта:**

   ```sh
   git clone https://github.com/foblako/EndlessSurvivors.git
   ```
![Вывод должен выглядеть примерно так:](https://github.com/foblako/EndlessSurvivors/blob/master/Readme%20images/image.png)

5. **Перейдите в директорию проекта:**

   ```sh
   cd EndlessSurvivors
   ```

6. **Создайте директорию сборки и перейдите в неё:**

   ```sh
   mkdir build && cd build
   ```
![В результате мы в папке проекта, осталось собрать игру](https://github.com/foblako/EndlessSurvivors/blob/master/Readme%20images/image-1.png)

7. **Запустите CMake для генерации файлов сборки:**

   ```sh
   cmake -G "MinGW Makefiles" ..
   ```

   ![Отлично, проект сконфигурирован](https://github.com/foblako/EndlessSurvivors/blob/master/Readme%20images/image-2.png)

8. **Соберите проект с помощью Make:**

   ```sh
   mingw32-make
   ```

   ![Успешная сборка игры должна дойти до 100% и завершиться](https://github.com/foblako/EndlessSurvivors/blob/master/Readme%20images/image-3.png)

9. **Запустите игру:**

   ```sh
   cd bin && main.exe
   ```