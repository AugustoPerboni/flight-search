# flight-search
This program helps you find the best airplane routes between the biggest airports in Europe, based on the airport's GPS location and commercial available routes. All the code was writen in C with special emphasis dymanic memory

## Program operation
The interaction between the user and the program is carried out through the command line. The program
has access to user requests through the arguments of the main function. The application must present, in the
screen, a listing of the information corresponding to the command that is specified by the user.
Example of commands:
- Vizualise airport information
  - main -aeroportos
- Display of direct flight information
  - main -voos
- Request for a direct route between airport A (LISBON) and airport B (ROME),
direct link (-L 0)
  - main LISBON ROME-L 0
- Identical to the previous point but the list is sorted by ascending departure time (-TC)
or descending (-TD)
  - main LISBON ROME -L 0 -TC
- Request for a route with a connection (but without the worry of a time connection)
  - main LISBON ROME -L 1
- Request for a route with a link (with concern for a time link)
  - main LISBON ROME -L 1 -TC
- Request for a connecting route (concerning the time connection) and with the lowest
distance
  - main LISBON ROME -L1 -TC -D
  
Note: “Time binding” means that the arrival time of a flight at an airport must be
less than the departure time of the connecting flight.
