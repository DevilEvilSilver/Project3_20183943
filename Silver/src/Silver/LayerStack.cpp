#include "pch.h"
#include "LayerStack.h"

namespace Silver {

	LayerStack::LayerStack()
	{
		m_LayerIterator = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer * layer)
	{
		m_LayerIterator = m_Layers.emplace(m_LayerIterator, layer);
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer * layer)
	{
		m_Layers.emplace_back(layer);
		layer->OnAttach();
	}

	void LayerStack::PopLayer(Layer * layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerIterator--;
		}
	}

	void LayerStack::PopOverlay(Layer * layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			layer->OnDetach();
			m_Layers.erase(it);
		}
	}

}