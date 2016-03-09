#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <time.h>


#define LARGEURGRILLE  60
#define HAUTEURGRILLE  50
#define TAILLECASEPIXEL 20
enum direction
{
	nord,
	ouest,
	sud,
	est,
};


int main(int argc, char *argv[])
{




	//****** Initialisation des variables ******
	SDL_Surface *ecran = NULL;
	SDL_Surface *corpsSerpent = NULL;
	SDL_Rect positionSerpent;
	SDL_Surface *insect = NULL;
	SDL_Rect positionInsect;


	SDL_Event event;
	int tempsPrecedent = 0, tempsActuel = 0;

	int i = 0;
	int j = 0;


	//La matrice principale du jeu
	int matrice[LARGEURGRILLE][HAUTEURGRILLE];

	

	//----- Serpent   -----
	// Tete du serpent
	short coorTeteX = 0;
	short coorTeteY = 0;
	int directionMouvSerpent = ouest;
	int compteurCorpsSerpent = 10; //Compteur que je met dans les case correspondant au corps du serpent 
	//ce compteur s'incrémentent pour chaque tête que j'ajoute, la queue est donc la case supérieur à 10 du tableau 
	// mais qui est le minimum de toute les autres case du corps du serpent
	
	//Queue du serpent
	short coordonneQueueX;
	short coordonneQueueY;

	//Variable temporaire pour la recherche de queue
	char queueTrouvee = 0;
	int minCompteur;

	//Si un changement de direction a été effectué ont interdit un nouveau changement tant qu'on a pas avancé au moins d'une case
	//Pour eviter de tourner sur soit meme 
	int changementDeDirectionAutorise = 1; // Par defaut Oui
	

	//---- Insecte ------
	short coordonneInsectX;
	short coordonneInsectY;

	char insectManger = 0;


	// ----- Perdu ------
	char perdu = 0;

	int continuer = 1;

	

	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("Snack !", NULL);

	//Création du fond en vert 
	ecran = SDL_SetVideoMode(LARGEURGRILLE * TAILLECASEPIXEL, HAUTEURGRILLE * TAILLECASEPIXEL, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 17, 206, 112));

	SDL_Flip(ecran); // Mise à jour de l'écran


	//Initialisation de la case bleue pour afficher le corps du serpent en reppetant autant de fois que necessaire cette case bleue
	corpsSerpent = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLECASEPIXEL, TAILLECASEPIXEL, 32, 0, 0, 0, 0);
	SDL_FillRect(corpsSerpent, NULL, SDL_MapRGB(ecran->format, 0, 20, 255)); 

	//Initialisation de l'insecte 
	insect = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLECASEPIXEL, TAILLECASEPIXEL, 32, 0, 0, 0, 0);
	SDL_FillRect(insect, NULL, SDL_MapRGB(ecran->format, 255, 106, 0)); 

	
	SDL_Flip(ecran); // Mise à jour de l'écran






	//intialisation à zero de la grille 
	for(i=0; i < LARGEURGRILLE; i ++)
	{
		for(j=0; j < HAUTEURGRILLE; j++)
		{
			matrice[i][j] = 0;
		}
	}

	//positionSerpentnement du serpent sur trois cases
	coorTeteX = 40; //Position du début choisis de façons arbitraire
	coorTeteY = 25;
	directionMouvSerpent = ouest;

	
	//Pour construire mon serpent je met dans les cases correspondantes à son corps dans la matrice
	// des valeurs qui s'inqurémente à partir de 10

	matrice[coorTeteX+2][coorTeteY] = compteurCorpsSerpent ++;
	matrice[coorTeteX+1][coorTeteY] = compteurCorpsSerpent ++;
	matrice[coorTeteX][coorTeteY] = compteurCorpsSerpent ++;



	//Positionnement du premier insect
	srand(time(NULL));
	do
	{
		coordonneInsectX = rand()%LARGEURGRILLE;
		coordonneInsectY = rand()%HAUTEURGRILLE;
	}while (matrice[coordonneInsectX][coordonneInsectY] > 0);  //On vérifie que la case choisie au hasard ne correspondent pas a une case du corps du serpent !! 

	matrice[coordonneInsectX][coordonneInsectY] = 1; 
	





	//Boucle principale du JEU 
	while (continuer) //Tant qu'on ne quite pas 
	{
		SDL_PollEvent(&event);
		switch(event.type)
		{
		case SDL_QUIT: //Event on quite Bye 
			continuer = 0;
			break;
		case SDL_KEYDOWN: //Event touche clavier tapée 
			switch(event.key.keysym.sym)
			{
			case SDLK_UP:
				if(directionMouvSerpent != sud && changementDeDirectionAutorise)
				{
					directionMouvSerpent = nord;
				}
				changementDeDirectionAutorise = 0;
				break;
			case SDLK_DOWN:
				if(directionMouvSerpent != nord && changementDeDirectionAutorise)
				{
					directionMouvSerpent = sud;
				}
				changementDeDirectionAutorise = 0;
				break;
			case SDLK_RIGHT:
				if(directionMouvSerpent != ouest && changementDeDirectionAutorise)
				{
					directionMouvSerpent = est;
				}
				changementDeDirectionAutorise = 0;
				break;
			case SDLK_LEFT:
				if(directionMouvSerpent != est && changementDeDirectionAutorise)
				{
					directionMouvSerpent = ouest;
				}
				changementDeDirectionAutorise = 0;
				break;
			default:
				;
			}
		default:
			;
		}


		//Gestion du temps 
		tempsActuel = SDL_GetTicks();
		if (tempsActuel - tempsPrecedent > 100) /* Si 100 ms se sont écoulées depuis le dernier tour de boucle */
		{
			if(perdu == 0)// Si perdu je ne bouge plus, si perdu est toujours à 0 je continue 
			{
				//On fait avancer le serpent
				switch(directionMouvSerpent) //Dans quelle direction faire avancer le serpent
				{
				case nord:
					if(matrice[coorTeteX][coorTeteY-1] == 1) //On va manger un insect !!
					{
						insectManger = 1;
					}

					if((coorTeteY-1 < 0) /*Sortie de cadre*/ || ( matrice[coorTeteX][coorTeteY-1] > 10) /*on se mange nous même*/)
					{
						perdu = 1;
					}
					else
					{
						//On avance dans la bonne direction
						//On met une valeur dans la case correspondante 
						matrice[coorTeteX][coorTeteY-1] = compteurCorpsSerpent ++;
						coorTeteY --;
					}
					break;
				case ouest:
					if(matrice[coorTeteX-1][coorTeteY] == 1) //On va manger un insect !!
					{
						insectManger = 1;
					}
					if((coorTeteX-1 < 0) /*Sortie de cadre*/ || ( matrice[coorTeteX-1][coorTeteY] > 10) /*on se mange nous même*/)
					{
						perdu = 1;
					}
					else
					{
						//On avance dans la bonne direction
						//On met une valeur dans la case correspondante 
						matrice[coorTeteX-1][coorTeteY] = compteurCorpsSerpent ++;
						coorTeteX --;
					}
					break;
				case sud:
					if(matrice[coorTeteX][coorTeteY+1] == 1) //On va manger un insect !!
					{
						insectManger = 1;
					}

					if((coorTeteY+1 >= HAUTEURGRILLE) /*Sortie de cadre*/ || ( matrice[coorTeteX][coorTeteY+1] > 10) /*on se mange nous même*/)
					{
						perdu = 1;
					}
					else
					{
						//On avance dans la bonne direction
						//On met une valeur dans la case correspondante 
						matrice[coorTeteX][coorTeteY+1] = compteurCorpsSerpent ++;
						coorTeteY ++;
					}
					break;
				case est:
					if(matrice[coorTeteX+1][coorTeteY] == 1) //On va manger un insect !!
					{
						insectManger = 1;
					}

					if((coorTeteX+1 >= LARGEURGRILLE) /*Sortie de cadre*/ || ( matrice[coorTeteX+1][coorTeteY] > 10) /*on se mange nous même*/)
					{
						perdu = 1;
					}
					else
					{
						//On avance dans la bonne direction
						//On met une valeur dans la case correspondante 
						matrice[coorTeteX+1][coorTeteY] = compteurCorpsSerpent ++;
						coorTeteX ++;
					}
					break;
				}

				//Apprès avoir avancer il faut supprimer la queue maintenant 

				//Si on a manger un insect on ne supprime pas la queue 
				if(insectManger != 1)
				{
					//On cherche la queue pour la supprimer  !!!! 
					coordonneQueueX = coorTeteX;
					coordonneQueueY = coorTeteY;
					minCompteur = compteurCorpsSerpent - 1;
					queueTrouvee = 0; // La queue n'est pas encore trouvée 
					while (!queueTrouvee)
					{
						if(matrice[coordonneQueueX+1][coordonneQueueY] == (minCompteur - 1)) //On cherche à droite
						{
							//bout de corps trouvé à droite ce n'est pas la queue ! 
							coordonneQueueX++;
							minCompteur--;
						}
						else if(matrice[coordonneQueueX-1][coordonneQueueY] == (minCompteur - 1))//Sinon on cherche  à gauche
						{
							//bout de corps trouvé à gauche ce n'est pas la queue ! 
							coordonneQueueX--;
							minCompteur--;
						}
						else if(matrice[coordonneQueueX][coordonneQueueY+1] == (minCompteur - 1))//Sinon on cherche au sud
						{
							//bout de corps trouvé au sud ce n'est pas la queue ! 
							coordonneQueueY++;
							minCompteur--;
						}
						else if(matrice[coordonneQueueX][coordonneQueueY-1] == (minCompteur - 1))//Sinon on cherche au nord
						{
							//bout de corps trouvé au nord ce n'est pas la queue ! 
							coordonneQueueY--;
							minCompteur--;
						}
						else
						{
							//bout de corps non trouvé !! c'est la queue Enfin !! :D
							matrice[coordonneQueueX][coordonneQueueY] = 0; // On l'efface, on remet un zero 
							queueTrouvee = 1; // On sort de la boucle
						}
					}
				}
				else // Si on a manger un insect il faut en créer un nouveau 
				{
					do
					{
						coordonneInsectX = rand()%LARGEURGRILLE;
						coordonneInsectY = rand()%HAUTEURGRILLE;
					}while (matrice[coordonneInsectX][coordonneInsectY] > 0);

					matrice[coordonneInsectX][coordonneInsectY] = 1; 
				}

				insectManger = 0; // On remet à zero l'indicateur indiquant qu'on a manger un insect


				// On autorise le changement de direction 
				changementDeDirectionAutorise = 1;

			}


			//******** Affichage de la matrice  ***********

			if(!perdu)
			{
				//Si je n'ai pas perdu je garde l'écran vert
				SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 17, 206, 112));
			}
			else
			{
				//Si j'ai perdu fond rouge !! 
				SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 10, 0));
			}

			for(i=0; i < LARGEURGRILLE; i ++)
			{
				for(j=0; j < HAUTEURGRILLE; j++)
				{
					if(matrice[i][j] >= 10) //Corps de serpent 
					{
						positionSerpent.x = i * TAILLECASEPIXEL; 
						positionSerpent.y = j * TAILLECASEPIXEL;
						SDL_BlitSurface(corpsSerpent, NULL, ecran, &positionSerpent); // Collage de la surface sur l'écran
						SDL_Flip(ecran); // Mise à jour de l'écran
					}
					else if(matrice[i][j] == 1) //Insect
					{
						positionInsect.x = i * TAILLECASEPIXEL; 
						positionInsect.y = j * TAILLECASEPIXEL;
						SDL_BlitSurface(insect, NULL, ecran, &positionInsect); // Collage de la surface sur l'écran
						SDL_Flip(ecran); // Mise à jour de l'écran
					}
				}
			}
			

			tempsPrecedent = tempsActuel; /* Le temps "actuel" devient le temps "precedent" pour nos futurs calculs */
		}

		
	}

	SDL_FreeSurface(corpsSerpent); // Libération de la surface
	SDL_FreeSurface(insect); // Libération de la surface
	SDL_FreeSurface(ecran);
	SDL_Quit();

	return EXIT_SUCCESS;
}