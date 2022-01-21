#include "alien.h"

bool Alien::m_HorizontalMove = true;

glm::vec2 Alien::unitGridSize = glm::vec2(0.0f);
glm::vec2 Alien::velocity = glm::vec2(0.0f);

Alien::Alien(AlienShape shape) : 
	Alien(glm::vec2(0.0f), glm::vec2(1.0f),	shape) {
}

Alien::Alien(glm::vec2 pos, glm::vec2 size, AlienShape shape)
	:	EntityDynamic(pos, size, "", glm::vec3(1.0f)), 
		shape(shape), m_SubSpriteNbr(0), m_TimeTracker(0), m_Descent(0.0f),
		m_AddSpriteNbr(true), m_ProcessedMoveChange(m_HorizontalMove), 
		m_LastPos(glm::vec2(0.0f))
{
	m_SubSprites = { '0', '1', '2' };

	switch (this->shape) {
		case SQUARE: {
			this->spriteName = "al_sq_0";
			this->color = glm::vec3(0.2, 1.0, 0.2);
			break;
		}
		case CIRCLE: {
			this->spriteName = "al_cl_0";
			this->color = glm::vec3(0.2, 0.2, 1.0);
			break;
		}
		case TRIANGLE: {
			this->spriteName = "al_tr_0";
			this->color = glm::vec3(1.0, 0.2, 1.0);
			break;
		}
	}
}

glm::vec2 Alien::Move(float dt, float window_width) {

	if (m_ProcessedMoveChange == m_HorizontalMove) {
		if (m_HorizontalMove) {

			float relativeBorder = window_width - this->size.x;
			if (this->position.x < 0 || this->position.x > relativeBorder) {
				m_HorizontalMove = false;
				this->velocity.x = -(this->velocity.x);
			}
			else {
				m_LastPos = this->position;
				this->position.x += this->velocity.x * dt;
			}
		}
		else {
			if (m_Descent < this->unitGridSize.y) {
				m_LastPos = this->position;
				this->position.y += this->velocity.y * dt;
				m_Descent += velocity.y * dt;
			}
			else {
				m_HorizontalMove = true;
			}
		}
	}
	else {
		m_Descent = 0;
		if (m_LastPos == this->position) {
			this->position.x = m_LastPos.x;
			this->position.y += this->velocity.y * dt;
		}

		if (m_LastPos.x != this->position.x) {
			this->position.x = m_LastPos.x;
			this->position.y += this->velocity.y * dt;
		}

		m_ProcessedMoveChange = m_HorizontalMove;
	}

	return this->position;
}

void Alien::Draw(SpriteRenderer& renderer) {
	m_TimeTracker += Renderer::GetDeltaTime();
	if (m_TimeTracker >= 0.75) {
		if (m_SubSpriteNbr + 1 == m_SubSprites.size())
			m_AddSpriteNbr = false;
		else if (m_SubSpriteNbr == 0)
			m_AddSpriteNbr = true;

		if (m_AddSpriteNbr)
			++m_SubSpriteNbr;
		else
			--m_SubSpriteNbr;

		m_TimeTracker = 0;
		this->spriteName.pop_back();
		this->spriteName += m_SubSprites[m_SubSpriteNbr];
	}

	Entity::Draw(renderer);
}