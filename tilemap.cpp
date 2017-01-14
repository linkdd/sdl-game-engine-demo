#include <sge/sge.hpp>
#include <iostream>
#include <memory>
#include <config.h>

using namespace std;
using namespace sge;

class RootNode : public Node
{
    using Node::Node;

    public:
        virtual vector<string> mro() const
        {
            auto _mro = Node::mro();
            _mro.push_back("RootNode");
            return _mro;
        }

        virtual void ready()
        {
            set_process(true);
        }

        virtual void process(Uint32 delta)
        {
            auto tmap = static_pointer_cast<TileMapNode>(get_node("tmap"));
            auto player = static_pointer_cast<AnimatedSpriteNode>(get_node("player"));

            SDL_Rect viewport = tmap->get_viewport();
            bool moving = false;

            if (engine.actions().is_action_pressed("ui_left"))
            {
                viewport.x -= 3;
                player->set_animation("rsrc/anim-left.json");
                moving = true;
            }

            if (engine.actions().is_action_pressed("ui_right"))
            {
                viewport.x += 3;
                player->set_animation("rsrc/anim-right.json");
                moving = true;
            }

            if (engine.actions().is_action_pressed("ui_up"))
            {
                viewport.y -= 3;
                player->set_animation("rsrc/anim-up.json");
                moving = true;
            }

            if (engine.actions().is_action_pressed("ui_down"))
            {
                viewport.y += 3;
                player->set_animation("rsrc/anim-down.json");
                moving = true;
            }

            if (!moving)
            {
                player->set_animation("rsrc/anim-sleep.json");
            }

            tmap->set_viewport(viewport);
        }
};

class TileMapDemoScene : public Scene
{
    public:
        virtual void load(Engine &engine)
        {
            auto tmap = make_shared<TileMapNode>("tmap"s, engine);
            tmap->set_tilemap("rsrc/map-layer0.json");
            tmap->set_viewport(8, 0, 256, 192);
            tmap->set_pos(4, 4);

            auto player = make_shared<AnimatedSpriteNode>("player"s, engine);
            player->set_animation("rsrc/anim-sleep.json");
            player->set_pos(50, 50);

            auto root = make_shared<RootNode>("root"s, engine);
            root->add_child(tmap);
            root->add_child(player);

            root_node = root;
        }

        virtual void unload(Engine &engine)
        {
            root_node.reset();
        }
};

int main()
{
    Configuration conf;
    int retcode = 0;

    conf.set("fps", 60)
        .set("display/width", 264)
        .set("display/height", 200)
        .set("display/fullscreen", true)
        .set("display/resizable", false)
        .set("display/scale", "linear")
        .set("assets/file/location", DEMO_SOURCE_DIR);

    try
    {
        Engine engine(conf);

        engine.actions().register_keyboard_action("ui_quit", SDLK_ESCAPE);
        engine.actions().register_keyboard_action("ui_left", SDLK_LEFT);
        engine.actions().register_keyboard_action("ui_left", SDLK_a);
        engine.actions().register_keyboard_action("ui_right", SDLK_RIGHT);
        engine.actions().register_keyboard_action("ui_right", SDLK_d);
        engine.actions().register_keyboard_action("ui_up", SDLK_UP);
        engine.actions().register_keyboard_action("ui_up", SDLK_w);
        engine.actions().register_keyboard_action("ui_down", SDLK_DOWN);
        engine.actions().register_keyboard_action("ui_down", SDLK_s);

        engine.mainloop().queue_process_handler(
            [&](Uint32 delta)
            {
                if (engine.actions().is_action_pressed("ui_quit"))
                {
                    engine.mainloop().quit();
                }
            }
        );

        auto mainscene = make_shared<TileMapDemoScene>();
        engine.scenes().add_scene("demo", mainscene);
        engine.scenes().switch_to_scene("demo");

        engine.mainloop().run();
    }
    catch (const Exception &e)
    {
        cerr << "Error: " << e.what() << endl;
        retcode = 1;
    }

    return retcode;
}
