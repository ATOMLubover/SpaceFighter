#ifndef _SCENE_END_H_
#define _SCENE_END_H_

#include "Scene.h"
#include "Camera.h"
#include "Timer.h"

#include <SDL_ttf.h>

#include <json.hpp>

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>

class SceneEnd : public Scene
{
public:
    SceneEnd();
    ~SceneEnd();

    void OnEnter() override;
    void OnExit() override;

    void OnHandle( SDL_Event& event ) override;
    void OnUpdate( float delta ) override;
    void OnRender( const Camera& cam ) override;

protected:
    void Refresh() override;

    void LoadResources() override;
    void UnloadResources() override;

private:
    void InitTimers();

    void RenderTypingPage( const Camera& cam );
    void RenderRankPage( const Camera& cam );

    void PopBackUtf8( std::string& str );

    void LoadRankingList();
    void SaveRankingList();

    void InsertRankingList( const std::string& id, int score );

private:
    static const std::string path_rank_save;

private:
    bool is_typing = true;

    std::string id_player;
    Timer timer_blink;
    bool is_underline_visible = true;

    TTF_Font* font_title = nullptr;
    TTF_Font* font_text = nullptr;

    std::map<int, std::string, std::greater<int>> ranking_list;
};

#endif // !_SCENE_END_H_