#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

constexpr double dim_max(1000.);                    // km
constexpr double rayon_min(50.);                    // km
constexpr double rayon_max(150.);                   // km
constexpr double display_rayon_robot(5);         	// km
constexpr double rayon_base(2);                     // km
constexpr double rayon_comm(300.);                  // km

constexpr double deltaD(5.);                        // km
constexpr double maxD_prosp(10 * dim_max);          // km
constexpr double maxD_forage(1.42 * dim_max);       // km
constexpr double maxD_transp(5 * dim_max);          // km
constexpr double maxD_com(1.42 * dim_max);          // km
constexpr unsigned int max_robots(3);               // km

constexpr double iniR(1000);                        // ktonne
constexpr double finR(10*iniR);                     // ktonne
constexpr double deltaR(iniR/4.);                   // ktonne
constexpr double cost_repair(0.0005);               // ktonne/km
constexpr double cost_prosp(iniR/100.);             // ktonne
constexpr double cost_forage(iniR/10.);             // ktonne
constexpr double cost_transp(iniR/10.);             // ktonne
constexpr double cost_com(iniR/1000.);              // ktonne

constexpr unsigned max_line(200);

#endif // CONSTANTES_H_INCLUDED
