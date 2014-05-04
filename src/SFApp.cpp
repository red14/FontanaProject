#include "SFApp.h"



SFApp::SFApp() : amove(1), score(0), is_running(true) 
{

  surface = SDL_GetVideoSurface();
  app_box = make_shared<SFBoundingBox>(Vector2(surface->w/2, surface->h/2), surface->w/2, surface->h/2);
  player  = make_shared<SFAsset>(SFASSET_PLAYER);
  auto player_pos = Point2(surface->w/2, 100.0f);
  player->SetPosition(player_pos);

  const int number_of_aliens = 40;
  for(int i=0; i<number_of_aliens; i++) 
  {
    int x=rand() %100 +1; cout << x <<endl;
    // place an alien at width/number_of_aliens * i
    auto alien = make_shared<SFAsset>(SFASSET_ALIEN);
    auto pos1   = Point2((x) * 10, 600.0f);
    alien->SetPosition(pos1);
    aliens.push_back(alien);
  }
  
  int x=rand() %100 +1; cout << x <<endl;
  auto coin = make_shared<SFAsset>(SFASSET_COIN);
  auto pos2  = Point2((x)* 13, 600.0f);
  coin->SetPosition(pos2);
  coins.push_back(coin);


auto barriers = make_shared<SFAsset>(SFASSET_BARRIER);
auto pos3  = Point2((x)* 8, 600.0f);
  barriers->SetPosition(pos3);
  barrier.push_back(barriers);
}


SFApp::~SFApp() 
{
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) 
{
  SFEVENT the_event = event.GetCode();
  switch (the_event) 
  {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;
  case SFEVENT_PLAYER_LEFT:
    player->GoWest();
    break;
  case SFEVENT_PLAYER_RIGHT:
    player->GoEast();
    break;
  case SFEVENT_FIRE:

    FireProjectile();
    break;
  }

}

int SFApp::OnExecute() 
{
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) 
  {
    // if this is an update event, then handle it in SFApp,
    // otherwise punt it to the SFEventDispacher.
    SFEvent sfevent((const SDL_Event) event);
    OnEvent(sfevent);
  }

}

void SFApp::OnUpdateWorld() 
{

    std::stringstream sstm;
    sstm << "score " << score ;
    SDL_WM_SetCaption(sstm.str().c_str(),  sstm.str().c_str());
    

  // Update projectile positions
  for(auto p: projectiles) 
  {
    p->ProgGoNorth();
  }

  for(auto c: coins) 
  {
    c->CGoSouth();
  }

  for(auto b: barrier) 
  {
    b->GoSouth();
  }

  // Update enemy positions
  for(auto a : aliens) 
  {
   if (amove == 1)
  {
    a->GoSouth();   
  }

}

  // Detect collisions
for(auto b: barrier) 
{
for(auto c: coins) 
 {
for(auto p : projectiles) 
  {
for(auto a : aliens) 
   {
      if(p->CollidesWith(a)) 
{
        p->HandleCollision();
        a->HandleCollision();
        score++;
}
      if (a->CollidesWith(player))
{
         //is_running = false;
}
     if (c->CollidesWith(player) || player->CollidesWith(c))
{
      c->HandleCollision();
      score = score +10;
}
     if (b->CollidesWith(player) || player->CollidesWith(b))
{
         is_running = false; 
}
   }
  }
 }
}

  // remove dead aliens (the long way)
  list<shared_ptr<SFAsset>> tmp;
  for(auto a : aliens) 
{
    if(a->IsAlive()) 
    {

      tmp.push_back(a);
   
    }

}
  aliens.clear();
  aliens = list<shared_ptr<SFAsset>>(tmp);

}

void SFApp::OnRender() 
{
  // clear the surface
  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 8, 25, 129) );
 // BG OnRender(surface);
  // draw the player
  player->OnRender(surface);

  for(auto p: projectiles) 
  {
    if(p->IsAlive()) {p->OnRender(surface);
  }
}

  for(auto a: aliens) 
  {
    if(a->IsAlive()) {a->OnRender(surface);
  }
}

  for(auto c: coins) 
  {
    if(c->IsAlive()) {c->OnRender(surface);
  }
}

   for (auto b : barrier)
   if (b->IsAlive()) {b->OnRender(surface);
  }

  // Switch the off-screen buffer to be on-screen
  SDL_Flip(surface);
}

void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}
