#include "Sounds.h"

using namespace std;
using namespace sf;


GameSound::GameSound()
{
	menuSound.openFromFile("files/musicmenu.ogg");
	level_0_ost.openFromFile("files/eepicm.ogg");
	level_1_ost.openFromFile("files/gameMusic1.ogg");
	level_2_ost.openFromFile("files/lvl2.ogg");
	rockCollideSound.openFromFile("resources/sound/shadow_collide.ogg");
	birdCollideSound.openFromFile("resources/sound/ghost_collide.wav");
	winSound.openFromFile("files/W.ogg");
	menuButtonSwitchSound.openFromFile("resources/sound/menu_button_switch.ogg");
	ammoBonusSound.openFromFile("resources/sound/bonus_weapon.ogg");
	healthBonusSound.openFromFile("resources/sound/bonus_health.ogg");
	boxBonusSound.openFromFile("resources/sound/bonus_box.wav");
	randomBonusSound.openFromFile("resources/sound/bonus_random.ogg");
	deathRock.openFromFile("resources/sound/shadow_death.ogg");
	deathSpider.openFromFile("resources/sound/spider_death.ogg");
	deathDragon.openFromFile("resources/sound/clown_death.ogg");
	deathBird.openFromFile("resources/sound/ghost_death.ogg");
	gameOverSound.openFromFile("files/Death.ogg");

	menuSound.setVolume(MAIN_MENU_VOLUME);
	level_0_ost.setVolume(GAMEPLAY_VOLUME);
	level_1_ost.setVolume(GAMEPLAY_VOLUME);
	level_2_ost.setVolume(GAMEPLAY_VOLUME);
	menuButtonSwitchSound.setVolume(MENU_BUTTON_SWITCH_VOLUME);
	gameOverSound.setVolume(MAIN_MENU_VOLUME);
	winSound.setVolume(WIN_VOLUME);

	deathRock.setVolume(COLLIDES_VOLIME);
	deathSpider.setVolume(COLLIDES_VOLIME);
	deathDragon.setVolume(COLLIDES_VOLIME);
	deathBird.setVolume(BIRD_DEATH_VOLUME);

	rockCollideSound.setVolume(COLLIDES_VOLIME);
	birdCollideSound.setVolume(COLLIDES_VOLIME);

	ammoBonusSound.setVolume(ADD_AMMO_BONUS_VOLUME);
	healthBonusSound.setVolume(ADD_HEALTH_BONUS_VOLUME);
	boxBonusSound.setVolume(ADD_HEALTH_BONUS_VOLUME);
	randomBonusSound.setVolume(ADD_AMMO_BONUS_VOLUME);
}

void GameSound::EnemyDeath(int type)
{
	switch (type)
	{
	case ROCK:
		deathRock.play();
		break;
	case DRAGON:
		deathDragon.play();
		break;
	case BIRD:
		deathBird.play();
		break;
	case SPIDER:
		deathSpider.play();
		break;
	default:
		break;
	}
}

void GameSound::CollideWithEnemy(int type)
{
	switch (type)
	{
	case ROCK:
		rockCollideSound.play();
		break;
	case DRAGON:
		break;
	case BIRD:
		birdCollideSound.play();
		break;
	case SPIDER:
		break;
	default:
		break;
	}
}

void GameSound::CollideWithBonus(int type)
{
	switch (type)
	{
	case AK_AMMO:
		PlayWithoutDouble(ammoBonusSound);
		break;
	case SHOOTGUN_AMMO:
		PlayWithoutDouble(ammoBonusSound);
		break;
	case HEALTH:
		PlayWithoutDouble(healthBonusSound);
		break;
	case RANDOM:
		PlayWithoutDouble(randomBonusSound);
		break;
	case BOX:
		PlayWithoutDouble(boxBonusSound);
		break;
	default:
		break;
	}
}

bool HeroSound::InitCharacterSound()
{
	if (!playerDeath.openFromFile("resources/sound/player_death.ogg") ||
		!weaponPistol.openFromFile("resources/sound/weapon_pistol.ogg") ||
		!weaponAK.openFromFile("resources/sound/weapon_ak.ogg") ||
		!weaponShootgun.openFromFile("resources/sound/weapon_shootgun.ogg") ||
		!switchWeapon.openFromFile("resources/sound/switch_weapon.ogg") ||
		!playerHurtGrunt.openFromFile("resources/sound/player_hurt_grunt.ogg") ||
		!emptyWeapon.openFromFile("resources/sound/empty_weapon.ogg"))
	{
		return false;
	}

	playerHurtGrunt.setVolume(COLLIDES_VOLIME);
	playerDeath.setVolume(PLAYER_DEATH_VOLUME);

	weaponPistol.setVolume(WEAPON_VOLUME);
	weaponAK.setVolume(WEAPON_VOLUME);
	weaponShootgun.setVolume(WEAPON_VOLUME);
	switchWeapon.setVolume(SWITCH_WEAPON_VOLUME);
	emptyWeapon.setVolume(SWITCH_WEAPON_VOLUME);

	return true;
}

bool BonusesSound::InitBonusesSound()
{
	return true;
}

void PlayWithoutDouble(sf::Music& audio)
{
	if (audio.getStatus() != sf::Music::Status::Playing)
	{
		audio.play();
	}
}