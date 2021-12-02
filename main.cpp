#include <iostream>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;


long convertIpToNumber(string ip)
{
    char c;
    int idx = 0;
    c = ip[idx];

    unsigned int integer;
    int val;
    int i, j = 0;
    for (j = 0; j < 4; j++)
    {
        if (!isdigit(c))
        { //first char is 0
            return (0);
        }
        val = 0;
        for (i = 0; i < 3; i++)
        {
            if (isdigit(c))
            {
                val = (val * 10) + (c - '0');
                c = ip[++idx];
            }
            else
                break;
        }
        if (val < 0 || val > 255)
        {
            return (0);
        }
        if (c == '.')
        {
            integer = (integer << 8) | val;
            c = ip[++idx];
        }
        else if (j == 3 && c == '\0')
        {
            integer = (integer << 8) | val;
            break;
        }
    }
    if (c != '\0')
    {
        return (0);
    }
    return integer;
}


string getOnlyIpPart(string ipAndPort)
{
    int portPosition = ipAndPort.find_first_of(':');
    string ip = ipAndPort.substr(0, portPosition);
    return ip;
}

int hashFunction(int IpNumber)
{
  if (IpNumber<0)
  {
    IpNumber=IpNumber*-1;
  }
  return IpNumber / 100;
}

int main()
{
  string month, hour, con, ipAndPortOrigen, ipAndPortDestino; 
  string ipBuscada;
  stringstream ss;
  long ipAsNumber;
  int key;
  int day;
  unordered_map<int, vector<string> > umap;
  vector<string>vstring;
  
  ifstream logFile("bitacora-5_2.txt");
  
  if (logFile.is_open())
    {
        // Read one line
        logFile >> month >> day >> hour >> ipAndPortOrigen; 

        // read the rest (and ignore it actually)
        getline(logFile, con);

        //Limpia el stringstream por si acaso
        ss.str(string());

        //crea un stringstream con los datos del registro
        ss <<month<<" "<<day<<" "<<hour<<" "<<ipAndPortOrigen<<con;

        //almacena los datos en un string para almacenarlos en un vector
        string resumen = ss.str();

        //Los almacena en el vector
        vstring.push_back(resumen);

        //Lee los datos de la ip, los convierte a int y les aplica la funcion hash
        string origenIpAddressAsString = getOnlyIpPart(ipAndPortOrigen);
        ipAsNumber=convertIpToNumber(origenIpAddressAsString);
        key=hashFunction(ipAsNumber);

        //introduce los datos como un par, esto es lo que se ocupa para un unordered map
        pair<int,vector<string>>myFirst(key,vstring);

        //Inserta el dato al mapa
        umap.insert(myFirst);

        //limpia el vector y esta listo para su siguiente ip
        vstring.clear();        

        // Continue with the rest of lines
        while (logFile >> month)
        {
            //leer archivo y almacenar
            logFile >> day >> hour >> ipAndPortOrigen;
            getline(logFile, con);

             //Limpia el stringstream
            ss.str(string());

            //crea un stringstream con los datos del registro
            ss <<month<<" "<<day<<" "<<hour<<" "<<ipAndPortOrigen<<con;

            //almacena los datos en un string para almacenarlos en un vector
            resumen = ss.str();

            //Lee los datos de la ip, los convierte a int y les aplica la funcion hash
            origenIpAddressAsString = getOnlyIpPart(ipAndPortOrigen);
            ipAsNumber=convertIpToNumber(origenIpAddressAsString);
            key=hashFunction(ipAsNumber);

            //empieza la insercion

            if (umap.find(key)==umap.end()) //  Si la key no existe dentro del mapa, la inserta
            {
              vstring.clear();//Limpia el vector
              vstring.push_back(resumen);//inserta el resumen al vector
              pair<int,vector<string>>hashPair(key,vstring);//crea el par de datos
              umap.insert(hashPair);//lo inserta en el mapa
            }

            else //De lo contrario, si ya exite esa key, solo actualiza su resumen
            {
              umap[key].push_back(resumen);
            }
        }//se termina el while
        logFile.close();
    }
    //se termina el if
    cout<<"Escriba la Ip que quiera buscar (solo la ip sin el puerto)"<<endl;
    cin>>ipBuscada; //pide al usuario la ip

    ipAsNumber=convertIpToNumber(ipBuscada); 
    key= hashFunction(ipAsNumber);

    if (umap.find(key)!=umap.end())
    {
      cout<<"Accesos: "<<umap[key].size()<<endl; //se imprimen los accesos
      for(int i=0;i<umap[key].size();i++)//se imprimen los resumenes de registros
      {
        cout<<umap[key][i]<<endl;
      }
    } 
    else
    {
      cout<<"Esa ip no existe en el sistema"<<endl;//si no existe se termina el programa
    }
} 