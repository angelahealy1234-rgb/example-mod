#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

// Global or static variable to hold our modifiable Pi value
static float g_customPi = 3.14159f;

class PiModifierPopup : public FLAlertLayer {
protected:
    TextInput* m_inputField;

    bool init() {
        if (!FLAlertLayer::init(350.f, 250.f, "Change Pi Value")) {
            return false;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Background layer
        auto bg = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
        bg->setContentSize({ 320.0f, 200.0f });
        bg->setPosition(winSize / 2);
        bg->setOpacity(200);
        m_mainLayer->addChild(bg);

        // Menu for buttons and inputs
        auto menu = CCMenu::create();
        menu->setPosition({ 0, 0 });
        m_mainLayer->addChild(menu);

        // Text input for Pi value
        m_inputField = TextInput::create(150.f, "Pi Value", "chatFont.fnt");
        m_inputField->setString(fmt::format("{}", g_customPi));
        m_inputField->setPosition({ winSize.width / 2, winSize.height / 2 + 20.f });
        m_mainLayer->addChild(m_inputField);

        // Apply Button
        auto applyBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Apply", "goldBtn_001.png", cocos2d::CCPoint{0,0}, 0.8f),
            this,
            menu_selector(PiModifierPopup::onApply)
        );
        applyBtn->setPosition({ winSize.width / 2 - 70.f, winSize.height / 2 - 50.f });
        menu->addChild(applyBtn);

        // Reset Button
        auto resetBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Reset", "accountId_btn_001.png", cocos2d::CCPoint{0,0}, 0.8f),
            this,
            menu_selector(PiModifierPopup::onReset)
        );
        resetBtn->setPosition({ winSize.width / 2 + 70.f, winSize.height / 2 - 50.f });
        menu->addChild(resetBtn);

        return true;
    }

    void onApply(CCObject*) {
        std::string valStr = m_inputField->getString();
        try {
            float newVal = std::stof(valStr);
            g_customPi = newVal;
            FLAlertLayer::create("Success", fmt::format("Pi changed to: {}", g_customPi), "OK")->show();
            log::info("Custom Pi set to: {}", g_customPi);
        } catch (...) {
            FLAlertLayer::create("Error", "Invalid number entered!", "OK")->show();
        }
    }

    void onReset(CCObject*) {
        g_customPi = 3.14159f;
        m_inputField->setString(fmt::format("{}", g_customPi));
        FLAlertLayer::create("Reset", "Pi reset to default (3.14159)", "OK")->show();
        log::info("Pi reset to default.");
    }

public:
    static void createPopup() {
        auto popup = new PiModifierPopup();
        if (popup && popup->init()) {
            popup->autorelease();
            popup->show();
        } else {
            delete popup;
        }
    }
};

// Hook into MenuLayer to add a button that opens our Pi modifier popup
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        auto menu = this->getChildByID("right-side-menu");
        if (menu) {
            auto piBtn = CCMenuItemSpriteExtra::create(
                CircleButtonSprite::createWithSpriteName("geode.loader/gear.png"),
                this,
                menu_selector(MyMenuLayer::onOpenPiMod)
            );
            piBtn->setID("pi-mod-button"_spr);
            menu->addChild(piBtn);
            menu->updateLayout();
        }

        return true;
    }

    void onOpenPiMod(CCObject*) {
        PiModifierPopup::createPopup();
    }
};
