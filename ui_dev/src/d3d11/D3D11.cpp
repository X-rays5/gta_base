// NOTE: taken from https://github.com/verrev/D3D11_Test_Env

#include "D3D11.h"

bool D3D11::initialize(const UINT &width, const UINT &height, const HWND &window, const bool &windowed)
{
	mWindowWidth = width;
	mWindowHeight = height;
	mWindow = window;
	mWindowed = windowed;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferDesc.Width = mWindowWidth;
	scd.BufferDesc.Height = mWindowHeight;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	scd.BufferCount = 1;
	scd.OutputWindow = mWindow;
	scd.Windowed = windowed;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels, 2, D3D11_SDK_VERSION, &scd, &mSwapChain, &mDevice, nullptr, &mDeviceContext);
	if (FAILED(hr)) {
		return false;
	}

	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&mSwapChainBuffer);
	if (FAILED(hr)) {
		return false;
	}

	hr = mDevice->CreateRenderTargetView(mSwapChainBuffer, nullptr, &mRenderTargetView);
	if (FAILED(hr)) {
		return false;
	}

	D3D11_TEXTURE2D_DESC dsdDesc;
	dsdDesc.Width = mWindowWidth;
	dsdDesc.Height = mWindowHeight;
	dsdDesc.MipLevels = 1;
	dsdDesc.ArraySize = 1;
	dsdDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsdDesc.SampleDesc.Count = 1;
	dsdDesc.SampleDesc.Quality = 0;
	dsdDesc.Usage = D3D11_USAGE_DEFAULT;
	dsdDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsdDesc.CPUAccessFlags = 0;
	dsdDesc.MiscFlags = 0;
	
	D3D11_VIEWPORT vp = { 0 };
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<FLOAT>(mWindowWidth);
	vp.Height = static_cast<FLOAT>(mWindowHeight);
	mDeviceContext->RSSetViewports(1, &vp);

	ID3D11RenderTargetView *renderTargetViews[] = { D3D11::mRenderTargetView };
	D3D11::mDeviceContext->OMSetRenderTargets(1, renderTargetViews, nullptr);

	return true;
}
void D3D11::endScene() const
{
	mSwapChain->Present(0, 0);
}
void D3D11::clearRenderTargetViews(const float &r, const float &g, const float &b, const float &a) const
{
	ID3D11RenderTargetView *rtvs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { nullptr };
	D3D11::mDeviceContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, rtvs, nullptr);
	for (auto&& rtv : rtvs) {
		if (rtv != nullptr) {
			float clearCols[] = { r, g, b, a };
			D3D11::mDeviceContext->ClearRenderTargetView(rtv, clearCols);
			safeRelease(rtv);
		}
	}
}
void D3D11::destroy() const
{
	mSwapChain->SetFullscreenState(0, nullptr);
	mDeviceContext->ClearState();
	safeRelease(mDevice);
	safeRelease(mDeviceContext);
	safeRelease(mSwapChain);
	safeRelease(mSwapChainBuffer);
	safeRelease(mRenderTargetView);
}