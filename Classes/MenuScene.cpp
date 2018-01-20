#include "MenuScene.h"
#include "GameplayScene.h"
#include "GameChoice.h"
#include "ShopScene.h"
#include "PlayerPrefs.h"
#include "Inventories.h"
#include "SimpleAudioEngine.h"
#include "BazinamaAcLe.h"

USING_NS_CC;
using namespace cocos2d::ui;

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

bool MenuScene::init(ValueMap& initData)
{
	if (!Layer::init())
		return false;

	_visibleOrigin = Director::getInstance()->getVisibleOrigin();
	_visibleSize = Director::getInstance()->getVisibleSize();

	auto inside = ImageView::create("gui/menu/menu_inside.png");
	addChild(inside);
	inside->setPosition(_visibleSize / 2);

	const int moveOffset = 700;
	const float moveTime = .5f;
	const float stopTime = 3;

	auto sandiwtches = Node::create();
	inside->addChild(sandiwtches);
	sandiwtches->setPosition(inside->getContentSize() / 2 + Size(moveOffset, 160));
	for (int i = 0; i < 4; ++i)
	{
		auto sand = Sprite::create(StringUtils::format("gui/menu/sandwitches/e%d.png", i + 1));
		sand->setScale(1.4f);
		sandiwtches->addChild(sand);
		sand->setPosition(Vect(i * moveOffset, 0));
	}

	auto resetLeft = CallFunc::create([=]() { sandiwtches->setPosition(inside->getContentSize() / 2 + Size(moveOffset, 160)); });
	auto moveSeq = Sequence::create(
		EaseSineOut::create(MoveBy::create(moveTime, Vect(-moveOffset, 0))),
		DelayTime::create(stopTime), EaseSineOut::create(MoveBy::create(moveTime, Vect(-moveOffset, 0))),
		DelayTime::create(stopTime), EaseSineOut::create(MoveBy::create(moveTime, Vect(-moveOffset, 0))),
		DelayTime::create(stopTime), EaseSineOut::create(MoveBy::create(moveTime, Vect(-moveOffset, 0))),
		DelayTime::create(stopTime), EaseSineOut::create(MoveBy::create(moveTime, Vect(-moveOffset, 0))),
		resetLeft, nullptr);
	sandiwtches->runAction(RepeatForever::create(moveSeq));

	_background = Layout::create();
	addChild(_background);
	_background->setContentSize(_visibleSize);
	_background->setAnchorPoint(Point::ANCHOR_MIDDLE);
	_background->setPosition(_visibleOrigin + _visibleSize / 2);

	auto backgImage = ImageView::create("gui/menu/menu_bg.png");
	_background->addChild(backgImage);
	backgImage->setPosition(_background->getContentSize() / 2);

	const int fontSize = 44;

	Size backgSize = _background->getContentSize();

	_playButton = Button::create("gui/menu/start.png");
	_background->addChild(_playButton);
	_playButton->setPosition(Vect(backgSize.width / 2, backgSize.height - 950));
	_playButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));
	_playButton->setScale(1.2f);

	_exitButton = Button::create("gui/menu/exitButton.png");
	_background->addChild(_exitButton);
	_exitButton->setPosition(Vect(_exitButton->getContentSize() / 2));
	_exitButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	_leaderboardButton = Button::create("gui/menu/leaderboardButton.png");
	_background->addChild(_leaderboardButton);
	_leaderboardButton->setPosition(Vect(backgSize.width / 2, _leaderboardButton->getContentSize().height / 2));
	_leaderboardButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	_settingButton = Button::create("gui/menu/settingButton.png");
	_background->addChild(_settingButton, 2);
	_settingButton->setPosition(Vect(backgSize.width - _settingButton->getContentSize().width / 2, _settingButton->getContentSize().height / 2));
	_settingButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	_shopButton = Button::create("gui/menu/shopButton.png", "gui/menu/shopButton_press.png");
	_background->addChild(_shopButton);
	_shopButton->setPosition(Vect(backgSize.width / 2, backgSize.height - _shopButton->getContentSize().height / 2));
	_shopButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	auto shopBadge = ImageView::create("gui/menu/shopBadge.png");
	_shopButton->addChild(shopBadge);
	shopBadge->setPosition(_shopButton->getContentSize() / 2 + Size(0, 5));

	_shopButton->setVisible(PlayerPrefs::getInstance().getSandwitchTotal() > 0);

	auto seq = Sequence::createWithTwoActions(ScaleTo::create(.25f, 1.1f), ScaleTo::create(.25f, 1));
	shopBadge->runAction(RepeatForever::create(seq));

	_settingLayout = Layout::create();
	_settingLayout->setContentSize(Size(110, 330));
	_background->addChild(_settingLayout);
	_settingLayout->setPosition(_settingButton->getPosition() + Vect(0, 70));
	_settingLayout->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);

	auto settingBackg = ImageView::create("gui/setting/frame.png");
	_settingLayout->addChild(settingBackg);
	settingBackg->setPosition(_settingLayout->getContentSize() / 2);

	auto volumeButton = Button::create(PlayerPrefs::getInstance().getVolume() ? "gui/setting/volume_on.png" : "gui/setting/volume_off.png");
	volumeButton->setName("volume");
	_settingLayout->addChild(volumeButton);
	volumeButton->setPosition(_settingLayout->getContentSize() / 2 + Size(0, 100));
	volumeButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	auto infoButton = Button::create("gui/setting/info.png");
	infoButton->setName("info");
	_settingLayout->addChild(infoButton);
	infoButton->setPosition(_settingLayout->getContentSize() / 2 + Size(0, 0));
	infoButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	auto creditsButton = Button::create("gui/setting/credits.png");
	creditsButton->setName("credits");
	_settingLayout->addChild(creditsButton);
	creditsButton->setPosition(_settingLayout->getContentSize() / 2 + Size(0, -100));
	creditsButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	_settingLayout->setPositionY(_settingLayout->getPositionY() - 600);
	_settingLayout->setVisible(false);

	//Game info popup
	auto infoPopup = Layout::create();
	infoPopup->setName("InfoPopup");
	infoPopup->setContentSize(_visibleSize);
	_background->addChild(infoPopup, 4);
	infoPopup->setTouchEnabled(true);
	infoPopup->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	infoPopup->setBackGroundColor(Color3B::BLACK);
	infoPopup->setBackGroundColorOpacity(150);

	auto infoBackg = ImageView::create("gui/setting/gameInfo.png");
	infoPopup->addChild(infoBackg);
	infoBackg->setPosition(infoPopup->getContentSize() / 2);

	infoPopup->addTouchEventListener([=](Ref* sender, Widget::TouchEventType eventType) {
		if (eventType == Widget::TouchEventType::ENDED)
			infoPopup->setVisible(false);
	});
	infoPopup->setVisible(false);

	//Credits popup
	auto creditsPopup = Layout::create();
	creditsPopup->setName("CreditsPopup");
	creditsPopup->setContentSize(_visibleSize);
	_background->addChild(creditsPopup, 4);
	creditsPopup->setTouchEnabled(true);
	creditsPopup->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	creditsPopup->setBackGroundColor(Color3B::BLACK);
	creditsPopup->setBackGroundColorOpacity(150);

	auto creditsBackg = ImageView::create("gui/gameover/finishedBackg.png");
	creditsPopup->addChild(creditsBackg);
	creditsBackg->setPosition(creditsPopup->getContentSize() / 2);
	creditsBackg->setScaleY(1.2f);

	auto creditsHeader = Text::create(GameChoice::getInstance().getString("TEXT_CREDITS"), GameChoice::getInstance().getFontName(), 80);
	creditsPopup->addChild(creditsHeader);
	creditsHeader->setPosition(creditsBackg->getPosition() + Vect(0, 270));
	creditsHeader->enableOutline(Color4B::GRAY, 3);

	creditsPopup->addTouchEventListener([=](Ref* sender, Widget::TouchEventType eventType) {
		if (eventType == Widget::TouchEventType::ENDED)
			creditsPopup->setVisible(false);
	});

	int cIndex = 0;
	auto crditsVec = GameChoice::getInstance().getCredits();
	for (auto c : crditsVec)
	{
		auto personText = Text::create(c, GameChoice::getInstance().getFontName(), 50);
		personText->setPosition(creditsBackg->getPosition() + Vect(0, 150 - (cIndex * 80)));
		personText->enableOutline(Color4B::GRAY, 2);
		creditsPopup->addChild(personText);
		cIndex++;
	}

	creditsPopup->setVisible(false);

	return true;
}

void MenuScene::onEnter()
{
	Layer::onEnter();

	if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/music_menu.ogg", true);

	if (!PlayerPrefs::getInstance().isTutorialFinished(2))
	{
		auto image = ImageView::create("tut/store.png");
		_background->addChild(image);
		image->setPosition(_background->getContentSize() / 2 + Size(0, 400));
		image->setScale(1.5f);
		image->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveBy::create(.5f, Vect(0, 20)), MoveBy::create(.5f, Vect(0, -20)))));
	}
}

void MenuScene::update(float dt)
{
	Layer::update(dt);
}

void MenuScene::buttonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::ENDED)
	{
		if (sender == _shopButton)
		{
			auto scene = TransitionSlideInR::create(.5f, ShopScene::createSceneData(ShopTypes::Food));
			Director::getInstance()->replaceScene(scene);
			PlayerPrefs::getInstance().finishTutrial(2);
		}
		if (!PlayerPrefs::getInstance().isTutorialFinished(2))
			return;

		if (sender == _playButton)
		{
			auto scene = TransitionFadeDown::create(1, GameplayScene::createSceneData(1));
			Director::getInstance()->replaceScene(scene);
		}
		if (sender == _exitButton)
		{
			Director::getInstance()->end();
		}
		if (sender == _leaderboardButton)
		{
			//show leaderboard
			BazinamaAcLe::getInstance().openLeaderBoard("pkgName", "scoeId", "timeScope");
		}
		if (sender == _settingButton)
		{
			const int actionTag = 1001;
			auto act = _settingButton->getActionByTag(actionTag);
			if (act == nullptr || act->isDone())
			{
				if (!_settingLayout->isVisible())
				{
					act = RotateBy::create(.5f, 180);
					act->setTag(actionTag);
					_settingButton->runAction(act);
					_settingLayout->setVisible(true);
					_settingLayout->runAction(MoveBy::create(.5f, Vect(0, 600)));
				}
				else
				{
					act = RotateBy::create(.5f, -180);
					act->setTag(actionTag);
					_settingButton->runAction(act);
					_settingLayout->runAction(MoveBy::create(.5f, Vect(0, -600)));
					runAction(Sequence::createWithTwoActions(DelayTime::create(.5f), CallFunc::create([=]() {_settingLayout->setVisible(false); })));
				}
			}
		}


		auto node = static_cast<Button*>(sender);
		if (node == _settingLayout->getChildByName("volume"))
		{
			bool on = !PlayerPrefs::getInstance().getVolume();
			PlayerPrefs::getInstance().setVolume(on);
			node->loadTextureNormal(on ? "gui/setting/volume_on.png" : "gui/setting/volume_off.png");
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(on ? GameChoice::getInstance().getMusicVolume() : 0);
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(on ? GameChoice::getInstance().getEffectVolume() : 0);
			
		}
		if (node == _settingLayout->getChildByName("info"))
		{
			auto infoPopup = _background->getChildByName("InfoPopup");
			infoPopup->setVisible(true);
		}
		if (node == _settingLayout->getChildByName("credits"))
		{
			auto creditsPopup = _background->getChildByName("CreditsPopup");
			creditsPopup->setVisible(true);
		}
	}
}
