#include <stdlib.h>
#include "pipeline.h"

int PuresoftPipeline::createTexture(const PURESOFTIMGBUFF32* image)
{
	size_t vacant = 0;
	for(; vacant < m_texPool.size(); vacant++)
	{
		if(!m_texPool[vacant])
			break;
	}

	if(m_texPool.size() == vacant)
	{
		m_texPool.push_back(NULL);
	}

	PuresoftFBO* fbo = new PuresoftFBO(image->width, image->scanline, image->height, image->elemLen, true, NULL, PuresoftFBO::WRAP);

	if(image->pixels)
	{
		memcpy(fbo->getBuffer(), image->pixels, fbo->getBytes());
	}

	m_texPool[vacant] = fbo;

	return (int)vacant;
}

void PuresoftPipeline::getTexture(int idx, PURESOFTIMGBUFF32* image)
{
	if(idx < 0 || idx >= (int)m_texPool.size())
	{
		throw std::out_of_range("PuresoftPipeline::getTexture");
	}

	PuresoftFBO* fbo = m_texPool[idx];

	image->width = fbo->getWidth();
	image->scanline = fbo->getScanline();
	image->height = fbo->getHeight();
	image->elemLen = fbo->getElemLen();
	image->pixels = fbo->getBuffer();
}

void PuresoftPipeline::destroyTexture(int idx)
{
	if(idx < 0 || idx >= (int)m_texPool.size())
	{
		throw std::out_of_range("PuresoftPipeline::destroyTexture");
	}

	if(m_texPool[idx])
	{
		delete m_texPool[idx];
		m_texPool[idx] = NULL;
	}
}