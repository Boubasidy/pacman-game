#ifndef PACMAN_HH
#define PACMAN_HH
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <exception>
#include <algorithm>
enum class direction {
    stop , droite , gauche , haut ,bas
};
class position {
public:
    position (unsigned int x , unsigned int y);
    unsigned int x() const {
        return _x ;
    }
    unsigned int y() const {
        return _y;
    }
    std::string tostring() const {
        return "("+std::to_string(x())+","+std::to_string(y())+")";
    }
    bool operator== (position const & pos) const {
        return ((x() == pos.x()) && (y() == pos.y()));
    }
    bool operator!=(position const & pos) const {
        return !operator==(pos);
    }
private:
    unsigned int _x ;
    unsigned int _y;
};
std::ostream & operator<<(std::ostream & os , position const & pos);

//classe taille

class taille {
public:
    taille (unsigned int h ,  unsigned int l );
    unsigned int w() const {
        return _largeur;
    }
    unsigned int h() const {
        return _hauteur ;
    }
    std::string tostring() const {
        return "("+std::to_string(_largeur)+","+std::to_string(_hauteur)+")";
    }
private:
    unsigned int _hauteur;
    unsigned int _largeur ;
    friend std::ostream & operator<<(std::ostream & os , taille const & tail);
};
class element {
public:
    element(position const & pos) : _pos(pos){}
    virtual taille tai() = 0 ;
    position pos() const {
        return _pos;
    }
    void setpos(position const & pos ){
        _pos = pos;
    }
    virtual char typeobjet() const = 0 ;
    virtual void sortieflux(std::ostream & os) {
            os<<"Objet de type ---> "<<typeobjet()<<","<<"de position ---> "<<pos().tostring()<<",";
            os<<" et de taille ---> "<<taillestring()<<",";

    }
    std::string taillestring() {
        return "("+std::to_string(tai().w())+","+std::to_string(tai().h())+")";
    }
    virtual std::unique_ptr<element> clone () const = 0 ;
   bool contient (element  & B);
   bool collision (element  & B);
private:
    position _pos;
};

class elementqsdep : public element {
public:
    elementqsdep(position const & pos , direction depl) : element(pos) , _direction(depl){}
    direction deplacement() const {
        return _direction ;
    }
    void afficher(direction d) const {
        switch (d) {
            case direction::bas:
                std::cout << "Bas" << std::endl;
                break;
            case direction::droite:
                std::cout << "Droite" << std::endl;
                break;
            case direction::gauche:
                std::cout << "Gauche" << std::endl;
                break;
            case direction::haut:
                std::cout << "Haut" << std::endl;
                break;
            case direction::stop:
                std::cout << "Stop" << std::endl;
                break;
            default:
                break;
        }
    }

    void setdir(direction dir) ;
    void sortieflux(std::ostream & os)  override{
        element::sortieflux(os);
        os<<" de direction ---> "; afficher(deplacement());
    }

private:
    direction _direction ;
};
class pacman : public elementqsdep {
public:
    pacman(position const & pos , direction dir);
    bool invincible() const {
        if (_niveau > 0) return true;
        else return false ;
    }
    void decrementeinvincibilite () {
        if (invincible()){
            _niveau -= 1 ;
        }
    }
    void devenirinvincible() {
        _niveau += 200;
    }
    taille tai() override;
    char typeobjet() const override ;
    void sortieflux(std::ostream & os) override{
        elementqsdep::sortieflux(os);
        os<<"Niveau d'invincibilité ---> " <<_niveau<<"\n";
    } ;
    std::unique_ptr<element> clone () const override{
        return std::make_unique<pacman>(*this);
    } ;
private:
     int _niveau ;
};

class fantome : public elementqsdep {
public:
    fantome(position const & pos , direction dir);
    taille tai() override{
        return taille(20,20);
    };
    char typeobjet() const override {
        return 'F';
    }

    std::unique_ptr<element> clone () const override {
        return std::make_unique <fantome>(*this);
    }

private:
};
class pacgomme : public element {
public:
    pacgomme(position const & pos):element(pos){}
    taille tai() override {
        return taille(3,3);
    }
    char typeobjet() const override {
        return 'G';
    }



};
class mur : public element {
public:
    mur(position const & pos , taille  & tail)
        :element(pos) , _tail((tail.w() > 10 && tail.h() > 10 )? tail : taille(11,11)){}
    taille tai() override {
        return _tail;
    }
    char typeobjet() const override {
        return 'M';
    }
    void sortieflux(std::ostream & os) override{
        element::sortieflux(os);
    } ;

    std::unique_ptr<element> clone () const override {
        return std::make_unique<mur>(*this);
    }
private:
     taille _tail;
};
std::ostream & operator<<(std::ostream & os , element & elt);

// class jeu
enum class etat {
    encours , defaite , victoire
};

class jeu {
public:
    jeu (jeu const & game );
    void ajouteelement (std::unique_ptr<element> p);
    void afficher(std::ostream &os);
    pacman & accespacman() const ;
    mur & accesmur() const ;
    void directionjoueur(direction d);
private:
    void appliquerdeplacementcollisionmur();
    void appliquerdeplacementcontact();
private:
    std::vector<std::unique_ptr<element>> _conteneur;
    etat _etat ;
};
class exceptionjeu : public std::exception {
public:
    exceptionjeu(std::string const & mess) : message(mess){} ;
    const char * what() const noexcept {
        return message.c_str();
    }
    std::string message ;
};

#endif // PACMAN_HH
