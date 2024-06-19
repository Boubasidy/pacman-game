#include "pacman.hh"
position::position(unsigned int x, unsigned int y)
    :_x(x) , _y(y){

}

std::ostream &operator<<(std::ostream &os, const position &pos){
    os<<pos.tostring()<<"\n";
    return os;
}
std::ostream &operator<<(std::ostream &os, const taille &tail){
    os<<tail.tostring()<<"\n";
    return os;
}

taille::taille(unsigned int h, unsigned int l)
    :_hauteur(h) , _largeur(l){

}

void elementqsdep::setdir(direction dir){
    _direction = dir;
}

std::ostream & operator<<(std::ostream & os , element& elt){
    elt.sortieflux(os);
    return os;
}

pacman::pacman(const position &pos, direction dir)
    :elementqsdep(pos,dir) , _niveau(0){
}

taille pacman::tai() {
    return taille(13,13);
}

char pacman::typeobjet() const{
    return 'P';
}


fantome::fantome(const position &pos, direction dir)
    :elementqsdep(pos,dir){}



bool element::contient(element &B) {
    return ((pos() == B.pos()) && ((tai().w() > B.tai().w()) && (tai().h() > B.tai().h())));
}

bool element::collision(element &B){
    return ((pos() == B.pos()) && ((tai().w() == B.tai().w()) && (tai().h() == B.tai().h())));
}


jeu::jeu(const jeu &game){
    for (auto const & p :game._conteneur){
        _conteneur.push_back(p->clone());
    }
}

void jeu::ajouteelement(std::unique_ptr<element> p)
{

            if (std::any_of(_conteneur.begin() , _conteneur.end() , [&p](auto const & elt) {
                            return p->collision(elt);
        })){
                throw exceptionjeu("l'element est en collision avec un autre");
            }
            else {
                _conteneur.push_back(std::move(p));
            }
        }


void jeu::afficher(std::ostream &os)
{
    for (auto const  & elt  : _conteneur){
        elt->sortieflux(os);
    }
}

pacman &jeu::accespacman() const{
auto it(std::find_if(_conteneur.begin() , _conteneur.end() , [](auto const & i){
    return i->typeobjet() == 'P';
}));
if(it != _conteneur.end()){
    return dynamic_cast<pacman &>(**it);
}
else {
    throw exceptionjeu("pas de pacman");
}
}

mur &jeu::accesmur() const{
    auto it(std::find_if(_conteneur.begin() , _conteneur.end() , [](auto const & i){
        return i->typeobjet() = 'm';
    } ));
            if (it != _conteneur.end()){
                return dynamic_cast<mur & >(**it);
            }else {
                throw exceptionjeu("pas de mur ");
            }
}

void jeu::directionjoueur(direction d)
{
    accespacman().setdir(d);
}

void jeu::appliquerdeplacementcollisionmur(){
    mur m = accesmur();
    std::for_each(_conteneur.begin() , _conteneur.end() , [&m] (auto const & i){
        if (((i->typeobjet() == 'F') || (i->typeobjet() == 'P')) & !i->collision(m)){
            switch(randirection()){
                                   case direction::haut:
                                       i->setpos(position(i->x(),i->y()-1));
                                       break;
                                   case direction::bas:
                                       i->setpos(position(i->x(),i->y()-1));
                                   break;
                               case direction::droite:
                                       i->setpos(position(i->x()+1,i->y()));
                                   break;
                               case direction::gauche:
                                   i->setpos(position(i->x()-1,i->y()));
                                   break;
                               case direction::stop:
                                   break;

              }
        }
    });
}
void jeu::appliquerdeplacementcontact(){
 pacman pack = accespacman();
 auto it (std::find_if(_ensembles.begin() , _ensembles.end() , [pack](auto const & i){
    if (i->typeobjet() == 'F' && i->collision(pack)){
        return true ;
    }else return false ;
}));
if(it != _ensembles.end() && pack.invincible()){
  _ensembles.erase(it,_ensembles.end());
}
if (it != _ensembles.end() && !pack.invincible()){
     _etat = jeu::etat::defaite;
 }

}


