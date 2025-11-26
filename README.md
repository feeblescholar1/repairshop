**Note: This was one of my university projects and it has been archived after submission.**


# repairshop - A user management program for car repair shops
This program can track individual clients, their cars and their operations performed on said cars.
The user can search within the database and list out operations with expiration dates due in 30 days.
Compilation does not require any external libraries. Documentation is written in **doxygen** comments.

*The following sections are translated from Hungarian.*

# How to use the program
No additional files are required to use the program. If you already have
a properly formatted database file (possibly from another computer or one handwritten
in the correct format),
place it in the same directory as the program with the name `export` and
the `.txt` extension.

## Input formats and handling

The program only accepts ASCII characters for its text input, and due to
the data storage structure, the pipe character (`|`) is reserved. The
program will notify the user of this and request that the input be
repeated without the indicated character.\
The input character limit and required format are also displayed.

Example (an input from the car management menu):\
`Rendszám (max. 15 karakter, formátum: ABCD1234):`

If the user exceeds the character limit, the program will cut the input
at the limit and use the preceding text as input.\
The program does **not** validate format correctness or data validity
(e.g., whether an email exists or if the license plate matches the
actual correct format). Incorrect formats may reduce search efficiency.

When the program requests an index number (e.g., a client's index), you
must enter the number shown at the beginning of the line. These numbers
are not unique identifiers and may change after operations. Except in
the search menu, numbering starts from zero and resets per type.
(Example: The first car belonging to client `27` has index `0`.)

Input formats: - License plates: `ABC123` or for newer plates
`ABCD123` - Dates: `YYYY-MM-DD HH:MM` Other inputs have no fixed format.

## Main menu

    [0] Kilépés                                        (Exit)
    [1] Ügyfelek kezelése                              (Manage clients -> Client management)
    [2] Keresés                                        (Search -> Search menu)
    [3] Az adatbázis nevének/leírásának szerkesztése   (Edit the database's name and description) 

To select an option, enter the number shown in brackets at the `Opció:`
prompt.\
If **option 3** is selected, the program will request the database name and
description.  \
Defaults: `(nincs név)` and `(nincs leírás)`.

## Client management

If the user selects *Ügyfelek kezelése*, they enter the client management
menu.

    [0] Vissza                        (Back -> Main menu)
    [1] Ügyfél hozzáadása             (Add a client)
    [2] Ügyfél adatainak módosítása   (Modify a client's data)
    [3] Ügyfél eltávolítása           (Remove a client)
    [4] Ügyfél autónak lekérdezése    (View client's cars)

Below the menu, the client list appears:  \
`[Index][Client name][Client email][Client phone number][Number of client’s cars]`\
If there are no clients, the program will state so.

When adding or modifying a client (**options 1 and 2**), the program requests the client's
data.\
When modifying or removing a client (**options 2 and 3**), the program requests the index
number.

## Car management

If *Ügyfél autónak lekérdezése* is selected, the car management menu appears.

    [0] Vissza                                (Back -> Client management)
    [1] Autó hozzáadása                       (Add a car)
    [2] Autó adatainak módosítása             (Modify a car's data)
    [3] Autó eltávolítása                     (Remove a car)
    [4] Javítás/vizsga hozzáadása             (Add a repair/inspection)
    [5] Javítás/vizsga adatainak módosítása   (Modify a repair's/inspection's data)
    [6] Javítás/vizsga eltávolítása           (Remove a repair/inspection)

The selected client's data, their cars, and repairs are shown:  \
`[Index][Car type][License plate]`\
    `[Index][Repair/inspection description][Creation date][Expiration date for inspections]`

For adding or modifying a car (**options 1 and 2**), the program requests the type and license
plate.\
For adding or modifying a repair (**options 4 and 5**), it requests the description, cost,
and---if it is an inspection---the expiration date. Leave the field
empty if no expiration date should be added.

## Search menu

If *Keresés* is selected in the main menu, the search menu appears.

    [0] Vissza                                        (Back -> Main menu)                                     
    [1] Ügyfél keresése                               (Search by client name)
    [2] Rendszám keresése                             (Search by plate number)
    [3] 30 napon belül lejáró vizsgák listázása       (List inspections expiring within 30 days)
    -------------------------------
    [4] Továbblépés az ügyfelek kezeléséhez           (Continue to client management -> Client management)
    [5] Továbblépés az autók/javítások kezeléséhez    (Continue to car/repair management -> Car management)

The program searches clients by name and cars by license plate.\
Search requires exact matches; wildcard characters (e.g., \* ?) are not
supported.

The program can list inspections expiring within 30 days, calculated
precisely relative to the current date and time.

**Options 4 and 5** lead to the previously described menus.\
Search results also display related owners/cars (e.g., searching a
license plate also shows its owner).
